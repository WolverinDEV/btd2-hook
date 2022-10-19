//
// Created by WolverinDEV on 19/10/2022.
//

#pragma once

#include <string>

namespace hooks::http {
    extern bool initialize(std::string& /* error */);
    extern void finalize();
}