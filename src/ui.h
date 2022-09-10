#pragma once

#include <string>

struct CursorPosition {
    bool active{false};

    float pos_x{0};
    float pos_y{0};
};

struct NetworkTick {
    uint32_t net_tick;
    uint64_t sim_tick;

    uint32_t latency;
    uint32_t jitter;

    uint64_t bloons_created;
    uint64_t bloons_leaked;
    uint64_t bloons_popped;
};

namespace ui {
    extern bool initialize(std::string& /* error */);
    extern void finalize();

    extern bool shown;
    extern bool consume_window_events;

    extern bool crash_player;

    namespace state {
        namespace string_decoder {
            extern std::string address_function;
            extern std::string key;

            extern std::string decode_result;
        }

        namespace debug {
            extern CursorPosition cursor_local;
            extern CursorPosition cursor_remote;

            extern NetworkTick network_tick_local;
            extern NetworkTick network_tick_remote;
        }
    }
}