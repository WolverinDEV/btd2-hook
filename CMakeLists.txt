cmake_minimum_required(VERSION 3.0.0)
project(BTD_Battles_2_Inject LANGUAGES CXX ASM_NASM VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

# include(CTest)
# enable_testing()


# The injector
add_executable(Injector injector/main.cpp)

include(third_party/imgui.cmake)

# The injected binary
add_subdirectory(third_party/fmt EXCLUDE_FROM_ALL)

add_library(BTDB2_Injectable SHARED
        src/main.cpp
        src/log.cpp
        src/util.cpp
        src/mem.cpp
        src/hook.cpp
        src/hooks/reflang.cpp
        src/payload_buffer.cpp
        src/game_event.cpp
        src/hook_win32_x64.asm
        src/ui.cpp
        src/hooks/http.cpp
        src/hooks/protocol.cpp
)
target_link_libraries(BTDB2_Injectable fmt::fmt imgui__core imgui__win32 imgui__opengl3)

find_program(POWERSHELL_PATH NAMES powershell)
add_custom_command(
    TARGET BTDB2_Injectable PRE_BUILD
    COMMAND ${POWERSHELL_PATH} "${CMAKE_SOURCE_DIR}/scripts/Eject.ps1"
)

add_custom_command(
    TARGET BTDB2_Injectable POST_BUILD
    COMMAND ${POWERSHELL_PATH} "${CMAKE_SOURCE_DIR}/scripts/Inject.ps1"
)
