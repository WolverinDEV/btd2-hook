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

    //size_t field_38;
    //bool read_error_occurred;
};

void send_network_message(const std::vector<uint8_t>& payload);
void send_game_event(game_event::GameEvent* /* event */);

extern std::map<uint32_t, std::string> game_event_mapping;
void handle_game_event(std::vector<uint8_t>& buffer, NetworkMessageBuffer& reader, bool is_send) {
    uint16_t event_id{};
    uint64_t sim_tick{}; // Maybe sim tick
    uint8_t unknown_u8{};

    auto read_offset{reader.read_offset()};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);
    reader.read_offset(read_offset);

    std::string event_name{};
    auto event_name_it = game_event_mapping.find(event_id);
    if(event_name_it == game_event_mapping.end()) {
        event_name = "<unknown event>";
    } else {
        event_name = event_name_it->second;
    }

    logging::info("{} game event 0x{:X} ({}) (Sim tick: 0x{:X}, Unknown U8: 0x{:X})", is_send ? "Sending" : "Receiving", event_id, event_name, sim_tick, unknown_u8);
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

    event->iter_fields([](const auto& name, const auto& type, const auto& value) {
        logging::info("  {} {} -> {}", type, name, value);
    });

    bool event_manipulated{false};
//    if(auto request = std::dynamic_pointer_cast<game_event::SNetPlaceTowerEvent>(event); request) {
//        logging::info("Game event upgrade!");
//        event_manipulated = true;
//    }
//    if(auto request = std::dynamic_pointer_cast<game_event::SNetScheduleRoundStartEvent>(event); request) {
//        logging::info("  Modyfing sim tick.");
//        request->round_starts_at_tick += 500;
//        event_manipulated = true;
//    }
    if(auto request = std::dynamic_pointer_cast<game_event::SNetPlaceTowerEvent>(event); request && is_send &&& ui::crash_player) {
        logging::info("  Sending emoji!");
        event_manipulated = true;

        // 3 is tack

//        game_event::SNetShowEmoteEvent eevent{0x7E, 0x00, 0xFF};
//        eevent.side = 1;
//        eevent.is_visual_emote = 1;
//        eevent.index = 1;
//        send_game_event(&eevent);
    }
    if(auto request = std::dynamic_pointer_cast<game_event::SNetReadyUpEvent>(event); request && is_send) {
        logging::info("  Setting map.");

//        game_event::SNetRequestMapSkipEvent eevent{0x11, 0x00, 0xFF};
//        eevent.explicitly_set_map_id = 17952145555794453232ull;
//        send_game_event(&eevent);
//
//        event_manipulated = true;
        // buffer.clear(); (Hmm cleaning the buffer will be an instant disconnect...)
    }

//    if(auto request = std::dynamic_pointer_cast<game_event::SNetSendLifeChangedEvent>(event); request && is_send) {
//        logging::info("  Sending full live.");
//        request->life_after = 150;
//        request->life_after = 150;
//        event_manipulated = true;
//    }
//
//    if(auto request = std::dynamic_pointer_cast<game_event::SNetSendCriticalLifeLostEvent>(event); request && is_send) {
//        logging::info("  Ignore crit live.");
//        buffer.clear();
////        request->life_after = 150;
////        request->life_after = 150;
////        event_manipulated = true;
//    }
    //
    //    17:44:42 [ INFO] Sending game event 0x6F (NetworkEvent::SNetUpgradeTowerEvent) (Unknown U64: 0x3A61, Unknown U8: 0x1)
//    17:44:42 [ INFO]   uint64_t sim_tick -> 14945
//    17:44:42 [ INFO]   uint8_t path -> 2
//    17:44:42 [ INFO]   int8_t to_level -> 2
//    17:44:42 [ INFO]   float_t cost -> 100
//    17:44:42 [ INFO]   float_t cost_xp -> 0
//    17:44:42 [ INFO]   uint64_t net_id -> 1020007
//    17:44:42 [ INFO]   uint32_t net_id_version -> 1
//    17:44:42 [ INFO]   uint32_t tower_type -> 3
//    17:44:42 [ INFO]   uint64_t eco_receipt_id -> 56
    // NetworkEvent::SNetEcoUpdatedEvent

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
    // allocate_pending_payload: 48 8D 05 ? ? ? ? 48 89 01 33 C0 48 89 41 08 48 89 41 10 48 89 41 18 48 89 41 20 48 89 41 28 48 89 41 30 48 89 41 38 66 89 41 40
    // free_pending_payload: 40 53 48 83 EC ? 48 8D 05 ? ? ? ? 48 8B D9 48 89 01 48 8B 49 08 48 85 C9 74 ? 48 8B 53 18 48 2B D1 48 81 FA ? ? ? ? 72 ? 4C 8B 41 F8 48 83 C2 ? 49 2B C8 48 8D 41 F8 48 83 F8 ? 77 ? 49 8B C8 E8 ? ? ? ? 33 C0 48 89 43 08 48 89 43 10 48 89 43 18 48 83 C4 ? 5B C3 FF 15 ? ? ? ? CC CC CC CC CC CC CC CC CC CC CC CC CC 48 8B C4 48 81 EC ? ? ? ? 0F 29 70 E8
    // MessageQueue::enqueue_send_message: see bellow
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


    // TODO: Hook earlier so we don't depend on the HTTP handler
    // TODO: Is the response actually editable?
    // 1.5.0 0x14515E6 + util::exe_offset
    // 1.5.1 90 48 8D 95 10 0B 00 00
    auto address_http_response = mem::find_pattern("90 48 8D 95 10 0B 00 00");
    if(!address_http_response) {
        logging::error("Failed to find http handle response function.");
        return false;
    }
    jump_hooks.push_back(hook::jump(*address_http_response, 0x0F, [](auto registers) {
        const auto& payload = *(std::string*) registers->rax;

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
        } else {
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