#include "./log.h"
#include "./util.h"
#include "./hook.h"
#include "./reflang.h"
#include "./game_event.h"
#include "./ui.h"
#include "mem.h"
#include <deque>
#include <Windows.h>
#include <atomic>
#include <map>

bool initialize();
void finalize();

[[maybe_unused]] BOOL WINAPI DllMain([[maybe_unused]] HINSTANCE handle_dll, DWORD call_reason, LPVOID ptr_reserved) {
    switch( call_reason ) {
        case DLL_PROCESS_ATTACH:
            return initialize();

        case DLL_PROCESS_DETACH:
            if(ptr_reserved) {
                /* Parent exe is terminating. */
                return true;
            }

            finalize();
            return true;

        default:
            /* Nothing to handle. */
            return true;
    }
}

struct NetworkMessage {
    void* vtable;

    std::vector<uint8_t> payload;
    std::atomic<uint8_t> ref_count;

    size_t write_offset;
    size_t read_offset;

    // Note: Not sure if these fields belong here.
    //size_t field_38;
    //bool read_error_occurred;
};

uint8_t local_player_id{};
uint8_t remote_player_id{};

void send_network_message(const std::vector<uint8_t>& payload);
void send_game_event(game_event::GameEvent* /* event */);

extern std::map<uint32_t, std::string> game_event_mapping;
void handle_game_event(std::vector<uint8_t>& buffer, NetworkMessageBuffer& reader, bool is_send) {
    uint16_t event_id{};
    uint64_t sim_tick{}; // Maybe sim tick
    uint8_t player_id{};

    auto read_offset{reader.read_offset()};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(player_id);
    reader.read_offset(read_offset);

    std::string event_name{};
    auto event_name_it = game_event_mapping.find(event_id);
    if(event_name_it == game_event_mapping.end()) {
        event_name = "<unknown event>";
    } else {
        event_name = event_name_it->second;
    }

    bool silent_event{false};
    if(event_name == "NetworkEvent::SNetTargetTouchPointEvent") {
        /* visible in net debug ui */
        silent_event = true;
    } else if(event_name == "NetworkEvent::SNetTickEvent") {
        /* visible in net debug ui */
        silent_event = true;
    }

    if(player_id != 0xFF) {
        if(is_send) {
            local_player_id = player_id;
        } else {
            remote_player_id = player_id;
        }
    }

    if(!silent_event) {
        logging::info("{} game event 0x{:X} ({}) (Sim tick: 0x{:X}, Player Id: 0x{:X})", is_send ? "Sending" : "Receiving", event_id, event_name, sim_tick, player_id);
    }
#ifdef GAME_EVENTS_DECLARED
    auto decoder = game_event::game_event_decoders.find(event_name);
    if(decoder == game_event::game_event_decoders.end()) {
        logging::warn("  Game event decoder unknown.");
        return;
    }

    auto event = std::shared_ptr{(*decoder->second)(reader)};
    if(!event) {
        logging::warn("  Game event failed to decode.");
        return;
    }

    if(!silent_event) {
        event->iter_fields([](const auto& name, const auto& type, const auto& value) {
            logging::info("  {} {} -> {}", type, name, value);
        });
    }

    if(auto request = std::dynamic_pointer_cast<game_event::SNetTargetTouchPointEvent>(event); request) {
        auto& cursor = is_send ? ui::state::debug::cursor_local : ui::state::debug::cursor_remote;
        cursor.active = !request->released;
        cursor.pos_x = request->point_x;
        cursor.pos_y = request->point_y;
    }

    if(auto request = std::dynamic_pointer_cast<game_event::SNetTickEvent>(event); request) {
        auto& state = is_send ? ui::state::debug::network_tick_local : ui::state::debug::network_tick_remote;

        state.net_tick = request->net_tick;
        state.sim_tick = request->sim_tick;

        state.latency = request->latency;
        state.jitter = request->jitter;

        state.bloons_created = request->bloons_created;
        state.bloons_popped = request->bloons_popped;
        state.bloons_leaked = request->bloons_leaked;
    }

    bool event_manipulated{false};

    if(auto request = std::dynamic_pointer_cast<game_event::SNetEcoUpdatedEvent>(event); request) {
        if(request->context == 9) {
            request->subcontext += 1;
            event_manipulated = true;
        }
    }

    if(auto request = std::dynamic_pointer_cast<game_event::SNetRequestQueueBloonSetEvent>(event); request) {
        request->bloon_set_id += 1;
        request->cost = 24;
        event_manipulated = true;
    }

//    if(auto request = std::dynamic_pointer_cast<game_event::SNetSendLifeChangedEvent>(event); request && is_send) {
//        logging::info("  Sending full live.");
//        request->life_after = 150;
//        request->life_after = 150;
//        event_manipulated = true;
//    }

//    if(auto request = std::dynamic_pointer_cast<game_event::SNetSendCriticalLifeLostEvent>(event); request && is_send) {
//        logging::info("  Ignore crit live.");
//        buffer.clear();
////        request->life_after = 150;
////        request->life_after = 150;
////        event_manipulated = true;
//    }

    if(event_manipulated) {
        NetworkMessageBuffer encoded{};
        (void) encoded.write(std::string{"GE"});
        if(!event->encode(encoded)) {
            logging::warn("  Failed to reencode game event...");
            return;
        }

        buffer = encoded.buffer();
    }
#endif
}

thread_local bool ignore_send_hook{false};
void* send_unknown_this{nullptr};
void* send_connection_handle{nullptr};

uintptr_t address_message_queue_enqueue_send_message{0};
uintptr_t address_allocate_pending_payload{0};
uintptr_t address_free_pending_payload{0};

bool initialize_network_address() {
    std::optional<uintptr_t> address{};

    address = mem::find_pattern("48 8D 05 ? ? ? ? 48 89 01 33 C0 48 89 41 08 48 89 41 10 48 89 41 18 48 89 41 20 48 89 41 28 48 89 41 30 48 89 41 38 66 89 41 40");
    if(!address.has_value()) {
        logging::error("Failed to find address for allocate_pending_payload");
        return false;
    }
    address_allocate_pending_payload = *address;

    address = mem::find_pattern("40 53 48 83 EC ? 48 8D 05 ? ? ? ? 48 8B D9 48 89 01 48 8B 49 08 48 85 C9 74 ? 48 8B 53 18 48 2B D1 48 81 FA ? ? ? ? 72 ? 4C 8B 41 F8 48 83 C2 ? 49 2B C8 48 8D 41 F8 48 83 F8 ? 77 ? 49 8B C8 E8 ? ? ? ? 33 C0 48 89 43 08 48 89 43 10 48 89 43 18 48 83 C4 ? 5B C3 FF 15 ? ? ? ? CC CC CC CC CC CC CC CC CC CC CC CC CC 48 8B C4 48 81 EC ? ? ? ? 0F 29 70 E8");
    if(!address.has_value()) {
        logging::error("Failed to find address for address_free_pending_payload");
        return false;
    }
    address_free_pending_payload = *address;

    // 1.5.0: 0x13B4F00 is the games send method
    //        0x13C3B00 is the message queue enqueue send payload packet.
    // 1.5.1: MessageQueue::enqueue_send_message pattern: 48 89 5C 24 20 55 56 57 41 56 41 57 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 40
    address = mem::find_pattern("48 89 5C 24 20 55 56 57 41 56 41 57 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 40");
    if(!address.has_value()) {
        logging::error("Failed to find address for MessageQueue::enqueue_send_message");
        return false;
    }
    address_message_queue_enqueue_send_message = *address;

    return true;
}


void send_network_message(const std::vector<uint8_t>& payload) {
    auto network_message_construct = (NetworkMessage*(*)(NetworkMessage*)) address_allocate_pending_payload;
    auto network_message_destruct = (NetworkMessage*(*)(NetworkMessage*, bool)) address_free_pending_payload;
    auto enqueue_fn = (bool(*)(void*, void*, void*)) address_message_queue_enqueue_send_message;

    auto message = (NetworkMessage*) malloc(sizeof(NetworkMessage) * 2);
    network_message_construct(message);

    message->payload = payload;
    message->write_offset = payload.size();

    ignore_send_hook = true;
    auto result = enqueue_fn(send_unknown_this, send_connection_handle, message);
    ignore_send_hook = false;
    logging::info("Send result: {}", result);

    network_message_destruct(message, false);
    // Note: Currently crashes the client...
    //       I'm not sure if we're not supposed to free the message (which is against what I observed
    //       or if something else is going on.
    // free(message);
}

void send_game_event(game_event::GameEvent* event) {
    NetworkMessageBuffer encoded{};
    (void) encoded.write(std::string{"GE"});
    if(!event->encode(encoded)) {
        logging::warn("  Failed to encode custom game event...");
        return;
    }

    send_network_message(encoded.buffer());
}

std::deque<std::shared_ptr<hook::JumpHook>> jump_hooks{};
bool initialize() {
    std::string error{};
    if(!logging::initialize(error)) {
        auto message = "Failed to initialize logging: \n" + error;
        MessageBox(nullptr, message.c_str(), "Log init failed", MB_OK);
        return false;
    }

    if(!util::initialize_offsets(error)) {
        logging::error("Failed to initialize offsets:\n{}", error);
        return false;
    }

    if(!reflang::initialize(error)) {
        logging::error("Failed to initialize reflang:\n{}", error);
        return false;
    }

    if(!ui::initialize(error)) {
        logging::error("Failed to initialize UI:\n{}", error);
        return false;
    }

    if(!initialize_network_address()) {
        return false;
    }

    logging::info("Device ID: {}", util::get_http_auth_device());
    logging::info("Session ID: {}", util::get_http_auth_session());
    logging::info("Http arg 2: {}", util::get_http_sign_arg2());

    auto address_http_execute_request = mem::find_pattern("40 55 53 56 57 41 54 41 56 41 57 48 8D 6C 24 80 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 70 49 8B F9");
    if(!address_http_execute_request.has_value()) {
        logging::error("Failed to find http execute request function.");
        return false;
    }
    jump_hooks.push_back(hook::jump(*address_http_execute_request, 0x10, [](auto registers) {
        const auto& target_url = *(std::string*) registers->rcx;
        const auto& payload = *(std::string*) registers->r8;

        logging::info("HTTP request to {}\n{}", target_url, payload);
    }));


#if 0
    /* Note: The offset only handles JSON results. The payload reference should be editable. */
    // 1.5.0 0x14515E6 + util::exe_offset
    // 1.5.1 90 48 8D 95 10 0B 00 00
    auto address_http_response = mem::find_pattern("90 48 8D 94 24 70 02 00 00 48 8D 8C 24 90 02 00 00 E8 ? ? ? ? 90 48 8B 9C 24 E0 02 00 00");
    if(!address_http_response) {
        logging::error("Failed to find http handle response function.");
        return false;
    }
    jump_hooks.push_back(hook::jump(*address_http_response, 0x0F, [](auto registers) {
        const auto& payload = *(std::string*) registers->rax;

        logging::info("HTTP response\n{}", payload);
    }));
#endif
    // HTTP response handle method (unclean: only for json results?). rdx (arg2) contains the http response.
    auto address_http_handle_json = mem::find_pattern("48 89 5C 24 18 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 F0 EF FF FF");
    if(!address_http_handle_json) {
        logging::error("Failed to find http handle response json function.");
        return false;
    }

    auto address_http_get_payload_string = mem::find_pattern("40 53 48 83 EC ? 4C 8B 81 C8 00 00 00");
    if(!address_http_get_payload_string) {
        logging::error("Failed to find http response get payload string.");
        return false;
    }

    logging::info("Override method: {:x}", *address_http_handle_json - util::exe_offset);
    auto http_get_payload_string = (std::string*(*)(void*, std::string*)) *address_http_get_payload_string;
    jump_hooks.push_back(hook::jump(*address_http_handle_json, 0x10, [http_get_payload_string](auto registers) {
        logging::info("HTTP Response {:x}", registers->rdx);

        /* This copies the response therefor it's not editable. */
        std::string payload{};
        http_get_payload_string((void*) registers->rdx, &payload);

        logging::info("HTTP response\n{}", payload);
    }));

    jump_hooks.push_back(hook::jump(address_message_queue_enqueue_send_message, 0x10, [](auto registers) {
        send_unknown_this = (void*) registers->rcx;
        send_connection_handle = (void*) registers->rdx;
        if(ignore_send_hook) {
            return;
        }

        auto payload = (NetworkMessage*) registers->r8;
        NetworkMessageBuffer reader{payload->payload};

        std::string packet_type{};
        (void) reader.read(packet_type);

        if(packet_type == "GE") {
            handle_game_event(payload->payload, reader, true);
        } else if(packet_type == "ECHO") {
            // TODO: What's about the payload?
            /* Don't log spamming echos. */
        }  else {
            logging::info("Send {} 0x{:X} (length: 0x{:X})", packet_type, (uintptr_t) payload, payload->write_offset);
        }
    }));

    // Method 13C3450 is the message queue enqueue recv payload packet
    auto address_me_enqueue_recv_message = mem::find_pattern("40 56 57 41 54 41 57 48 83 EC ? 4C 8B F9");
    if(!address_me_enqueue_recv_message) {
        logging::error("Failed to find message queue enqueue recv function.");
        return false;
    }

    jump_hooks.push_back(hook::jump(*address_me_enqueue_recv_message, 0x5E - 0x50, [](auto registers) {
        auto payload = (NetworkMessage*) registers->r8;

        NetworkMessageBuffer reader{payload->payload};

        std::string packet_type{};
        (void) reader.read(packet_type);

        if(packet_type == "GE") {
            handle_game_event(payload->payload, reader, false);
        } else if(packet_type == "ECHO") {
            /* Don't log spamming echos. */
        } else {
            logging::info("Received {} 0x{:X} (length: 0x{:X})", packet_type, (uintptr_t) payload, payload->write_offset);
        }
    }));

    logging::info("Inject success!");
    return true;
}

void finalize() {
    ui::finalize();
    jump_hooks.clear();
    logging::info("Shutting down succeeded.");
}