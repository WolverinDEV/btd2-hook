//
// Created by WolverinDEV on 19/10/2022.
//
#include "./protocol.h"
#include "../mem.h"
#include "../log.h"
#include "../game_event.h"
#include "../hook.h"
#include "../ui.h"

#include <vector>
#include <optional>
#include <atomic>

extern std::map<uint32_t, std::string> game_event_mapping;

namespace hooks::protocol {
    thread_local bool ignore_send_hook{false};
    void* send_unknown_this{nullptr};
    void* send_connection_handle{nullptr};

    uintptr_t address_message_queue_enqueue_send_message{0};
    uintptr_t address_message_queue_enqueue_recv_message{0};
    uintptr_t address_allocate_pending_payload{0};
    uintptr_t address_free_pending_payload{0};

    std::shared_ptr<hook::JumpHook> hook_enqueue_send{};
    std::shared_ptr<hook::JumpHook> hook_enqueue_recv{};

    uint8_t local_player_id{};
    uint8_t remote_player_id{};

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

    void handle_game_event(std::vector<uint8_t>& buffer, NetworkMessageBuffer& reader, bool is_send);
    bool initialize(std::string& error) {
        /* resolve required addresses */
        {
            std::optional<uintptr_t> address{};

            address_allocate_pending_payload = mem::find_pattern("48 8D 05 ? ? ? ? 48 89 01 33 C0 48 89 41 08 48 89 41 10 48 89 41 18 48 89 41 20 48 89 41 28 48 89 41 30 48 89 41 38 66 89 41 40").value_or(0);
            if(!address_allocate_pending_payload) {
                logging::error("Failed to find address for allocate_pending_payload");
                return false;
            }

            address_free_pending_payload = mem::find_pattern("40 53 48 83 EC ? 48 8D 05 ? ? ? ? 48 8B D9 48 89 01 48 8B 49 08 48 85 C9 74 ? 48 8B 53 18 48 2B D1 48 81 FA ? ? ? ? 72 ? 4C 8B 41 F8 48 83 C2 ? 49 2B C8 48 8D 41 F8 48 83 F8 ? 77 ? 49 8B C8 E8 ? ? ? ? 33 C0 48 89 43 08 48 89 43 10 48 89 43 18 48 83 C4 ? 5B C3 FF 15 ? ? ? ? CC CC CC CC CC CC CC CC CC CC CC CC CC 48 8B C4 48 81 EC ? ? ? ? 0F 29 70 E8").value_or(0);
            if(!address_free_pending_payload) {
                logging::error("Failed to find address for address_free_pending_payload");
                return false;
            }

            // 1.5.0: 0x13B4F00 is the games send method
            //        0x13C3B00 is the message queue enqueue send payload packet.
            // 1.5.1: MessageQueue::enqueue_send_message pattern: 48 89 5C 24 20 55 56 57 41 56 41 57 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 40
            address_message_queue_enqueue_send_message = mem::find_pattern("48 89 5C 24 20 55 56 57 41 56 41 57 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 40").value_or(0);
            if(!address_message_queue_enqueue_send_message) {
                logging::error("Failed to find address for MessageQueue::enqueue_send_message");
                return false;
            }

            // Method 13C3450 is the message queue enqueue recv payload packet
            address_message_queue_enqueue_recv_message = mem::find_pattern("40 56 57 41 54 41 57 48 83 EC ? 4C 8B F9").value_or(0);
            if(!address_message_queue_enqueue_recv_message) {
                logging::error("Failed to find address for MessageQueue::enqueue_recv_message");
                return false;
            }
        }

        /* Hook network packet enqueues */
        {
            hook_enqueue_send = hook::jump(address_message_queue_enqueue_send_message, 0x10, [](auto registers) {
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
            });

            hook_enqueue_recv = hook::jump(address_message_queue_enqueue_recv_message, 0x5E - 0x50, [](auto registers) {
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
            });
        }
        return true;
    }

    void finalize() {
        hook_enqueue_recv = nullptr;
        hook_enqueue_send = nullptr;
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
        /* NOTE: Manipulate event here */
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
}