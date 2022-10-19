//
// Created by WolverinDEV on 19/10/2022.
//

#pragma once

#include <string>

namespace game_event {
    struct GameEvent;
}

namespace hooks::protocol {
    extern uint8_t local_player_id;
    extern uint8_t remote_player_id;

    extern bool initialize(std::string& /* error */);
    extern void finalize();

    extern void send_game_event(game_event::GameEvent* event);
}