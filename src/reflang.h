#pragma once

#include <string>

namespace reflang {
    extern bool initialize(std::string& /* error */);

    extern void dump_event_map();
    extern void generate_game_events();
}