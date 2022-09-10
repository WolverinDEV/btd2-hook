#pragma once

#include <string>

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
    }
}