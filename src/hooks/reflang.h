#pragma once

#include <string>

namespace hooks::reflang {
    extern bool initialize(std::string& /* error */);
    extern void finalize();

    extern void dump_event_map();
    extern void generate_game_events();
}