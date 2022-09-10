#include "./ui.h"
#include "./hook.h"
#include "util.h"
#include "game_event.h"
#include "log.h"
#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_opengl3_loader.h>
#include <imgui_impl_opengl3.h>

HWND hGameWindow;
WNDPROC hGameWindowProc;
std::shared_ptr<hook::JumpHook> swap_buffers_hook{};

bool ui::shown{true};
bool ui::consume_window_events{false};
bool ui::crash_player{false};
void hooked_wglSwapBuffers(HDC hDc);

using namespace ui;

bool ui::initialize(std::string &error) {
    auto handle_opengl32 = GetModuleHandle("opengl32.dll");
    if(!handle_opengl32) {
        error = "Missing opengl32.dll module";
        return false;
    }

    auto fn_swap_buffers = (uintptr_t) GetProcAddress(handle_opengl32, "wglSwapBuffers");
    if(!fn_swap_buffers) {
        error = "Can not find wglSwapBuffers function";
        return false;
    }

    // Advance by 5 bytes since Steam already hooks this function with a relative jump.
    swap_buffers_hook = hook::jump(fn_swap_buffers + 5, 0xE4 - 0xD5, [](auto registers) {
        hooked_wglSwapBuffers((HDC) registers->rcx);
    });

    (void) swap_buffers_hook; // So it appears as used for the linter.
    return true;
}

void ui::finalize() {
    swap_buffers_hook = nullptr;
    auto original_window_proc = std::exchange(hGameWindowProc, nullptr);
    if(original_window_proc) {
        SetWindowLongPtr(hGameWindow, GWLP_WNDPROC, (LONG_PTR) original_window_proc);
    }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK windowProc_hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_KEYDOWN && wParam == VK_F2) {
        ui::shown = !ui::shown;
        return true;
    }

    if(ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) {
        return true;
    }

    if(ui::shown && ui::consume_window_events) {
        return true;
    }

    return CallWindowProc(hGameWindowProc, hWnd, uMsg, wParam, lParam);
}

void ui_render();
void hooked_wglSwapBuffers(HDC hDc) {
    // Initialize glew and imgui but only once
    static bool imGuiInitialized = false;
    if (!imGuiInitialized) {
        imGuiInitialized = true;

        hGameWindow = WindowFromDC(hDc);
        hGameWindowProc = (WNDPROC) SetWindowLongPtr(hGameWindow, GWLP_WNDPROC, (LONG_PTR)windowProc_hook);

        imgl3wInit();
        ImGui::CreateContext();
        ImGui_ImplWin32_Init(hGameWindow);
        ImGui_ImplOpenGL3_Init();
        ImGui::StyleColorsDark();
        ImGui::GetStyle().AntiAliasedFill = false;
        ImGui::GetStyle().AntiAliasedLines = false;
        ImGui::CaptureMouseFromApp();
        ImGui::GetStyle().WindowTitleAlign = ImVec2(0.5f, 0.5f);

        auto& io = ImGui::GetIO();
        io.FontGlobalScale = 2;
    }

    if (ui::shown) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();

        ImGui::NewFrame();
        {
            ui_render();

            ImGui::Begin("Basic Settings");
            ImGui::Checkbox("Consume Window events", &ui::consume_window_events);
            ImGui::Checkbox("Crash players", &ui::crash_player);
            ImGui::End();
        }
        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}

std::string state::string_decoder::address_function{};
std::string state::string_decoder::key{};
std::string state::string_decoder::decode_result{};

CursorPosition state::debug::cursor_local{};
CursorPosition state::debug::cursor_remote{};
NetworkTick state::debug::network_tick_local{};
NetworkTick state::debug::network_tick_remote{};

extern void send_game_event(game_event::GameEvent* /* event */);
extern uint8_t local_player_id;
extern uint8_t remote_player_id;

static constexpr auto kTemplateAuthClipboard{R"(
USER_SESSION = {
    "session": "%session%",
    "device": "%device%",
    "skuID": 1704,
    "appID": 17
}

APP_HTTP_SIGN_ARG2 = "%http_sign_arg2%"
)"};

void ui_render() {
    {
        ImGui::Begin("String decoder");

        ImGui::InputText("Address", &state::string_decoder::address_function);
        ImGui::InputText("Key", &state::string_decoder::key);
        if(ImGui::Button("Decode")) {
            if(state::string_decoder::key.find("0x") != 0) {
                state::string_decoder::decode_result = "- invalid key -";
            } else {
                uintptr_t address = std::stoull(state::string_decoder::address_function, nullptr, 16);
                uint64_t key = std::stoull(state::string_decoder::key.substr(2), nullptr, 16);
                if(address == 0) {
                    state::string_decoder::decode_result = "- invalid address -";
                } else if(key == 0) {
                    state::string_decoder::decode_result = "- invalid key -";
                } else {
                    state::string_decoder::decode_result = util::decode_xor_string(key, address + util::exe_offset, std::nullopt);
                }
            }
        }

        ImGui::SameLine();
        if(ImGui::Button("Copy Result")) {
            ImGui::LogToClipboard();
            ImGui::LogText("%s", state::string_decoder::decode_result.c_str());
            ImGui::LogFinish();
        }

        ImGui::Dummy(ImVec2(0.0f, 20.0f));
        ImGui::Text("Decode result:");
        ImGui::TextWrapped("%s", state::string_decoder::decode_result.c_str());

        ImGui::End();
    }

    {
        ImGui::Begin("Game Menu");
        if(ImGui::Button("Copy Auth")) {
            auto generated = util::replace_all(kTemplateAuthClipboard, {
                    { "%session%", util::get_http_auth_session() },
                    { "%device%", util::get_http_auth_device() },
                    { "%http_sign_arg2%", util::get_http_sign_arg2() },
            });

            ImGui::LogToClipboard();
            ImGui::LogText("%s", generated.c_str());
            ImGui::LogFinish();
        }

        if(ImGui::Button("Skip lobby 1")) {
            game_event::SNetReadyUpEvent eevent{0x0C, 0x00, 0xFF};
            eevent.ready_phase = 1;
            eevent.lobby_generation = 0;
            send_game_event(&eevent);
        }

        if(ImGui::Button("Skip lobby 2")) {
            game_event::SNetReadyUpEvent eevent{0x0C, 0x00, 0xFF};
            eevent.ready_phase = 2;
            eevent.lobby_generation = 0;
            send_game_event(&eevent);
        }

        if(ImGui::Button("Switch map")) {
            {
//                game_event::SNetRequestMapSkipEvent eevent{0x11, 0x00, 0xFF};
//                eevent.explicitly_set_map_id = 14430652889006179982ull;
//                send_game_event(&eevent);
            }

            {
                game_event::SNetShowEmoteEvent eevent{0x7E, 0x00, 0xFF};
                eevent.side = 1;
                eevent.is_visual_emote = 1;
                eevent.index = 2;
                send_game_event(&eevent);
            }

            {
                // std::vector<uint8_t> player_ids_by_side -> 1, 2
                logging::debug("Local player {}, remote player {}", local_player_id, remote_player_id);
                {
                    game_event::SNetEcoUpdatedEvent eevent{0x85, 0, local_player_id};
                    eevent.cash = 930;
                    eevent.income = 251;
                    eevent.eco_receipt_id = 1233;
                    eevent.context = 9; // Ballon send
                    eevent.subcontext = 24; // Which ballon
                    send_game_event(&eevent);
                }
                {
                    game_event::SNetRequestQueueBloonSetEvent eevent{0x71, 0, local_player_id};
                    eevent.target_side = 1;
                    eevent.sim_tick = 0;

                    eevent.bloon_set_id = 24;
                    eevent.bloon_features = 0;
                    eevent.path_index = 0;

                    eevent.cost = 1000;
                    eevent.income_change = 1.8;

                    eevent.eco_receipt_id = 1233;
                    eevent.self_send_flag = 0;

                    send_game_event(&eevent);

//                    16:38:58 [ INFO] Sending game event 0x85 (NetworkEvent::SNetEcoUpdatedEvent) (Sim tick: 0x5F60, Player Id: 0x1)
//                    16:38:58 [ INFO]   float_t cash -> 12970.106
//                    16:38:58 [ INFO]   float_t income -> 262.5
//                    16:38:58 [ INFO]   uint64_t eco_receipt_id -> 90
//                    16:38:58 [ INFO]   unknown_8 context -> 9
//                    16:38:58 [ INFO]   uint64_t subcontext -> 24
//                    16:38:58 [ INFO] Sending game event 0x71 (NetworkEvent::SNetRequestQueueBloonSetEvent) (Sim tick: 0x0, Player Id: 0x1)
//                    16:38:58 [ INFO]   uint8_t target_side -> 0
//                    16:38:58 [ INFO]   uint64_t sim_tick -> 0
//                    16:38:58 [ INFO]   uint32_t bloon_set_id -> 24
//                    16:38:58 [ INFO]   uint16_t bloon_features -> 0
//                    16:38:58 [ INFO]   uint32_t path_index -> 0
//                    16:38:58 [ INFO]   float_t cost -> 1000
//                    16:38:58 [ INFO]   float_t income_change -> 0
//                    16:38:58 [ INFO]   uint64_t eco_receipt_id -> 90
//                    16:38:58 [ INFO]   uint8_t self_send_flag -> 0

//                    16:37:06 [ INFO] Sending game event 0x85 (NetworkEvent::SNetEcoUpdatedEvent) (Sim tick: 0x0, Player Id: 0x1)
//                    16:37:06 [ INFO]   float_t cash -> 930
//                    16:37:06 [ INFO]   float_t income -> 251
//                    16:37:06 [ INFO]   uint64_t eco_receipt_id -> 1233
//                    16:37:06 [ INFO]   unknown_8 context -> 9
//                    16:37:06 [ INFO]   uint64_t subcontext -> 9
//                    16:37:06 [ INFO] Send result: true
//                    16:37:06 [ INFO] Sending game event 0x71 (NetworkEvent::SNetRequestQueueBloonSetEvent) (Sim tick: 0x0, Player Id: 0x0)
//                    16:37:06 [ INFO]   uint8_t target_side -> 0
//                    16:37:06 [ INFO]   uint64_t sim_tick -> 0
//                    16:37:06 [ INFO]   uint32_t bloon_set_id -> 9
//                    16:37:06 [ INFO]   uint16_t bloon_features -> 0
//                    16:37:06 [ INFO]   uint32_t path_index -> 0
//                    16:37:06 [ INFO]   float_t cost -> 30
//                    16:37:06 [ INFO]   float_t income_change -> 1.8
//                    16:37:06 [ INFO]   uint64_t eco_receipt_id -> 1233
//                    16:37:06 [ INFO]   uint8_t self_send_flag -> 0
//                    16:37:06 [ INFO] Send result: true

//                    16:21:50 [ INFO] Sending game event 0x85 (NetworkEvent::SNetEcoUpdatedEvent) (Sim tick: 0x197B, Unknown U8: 0x1)
//                    16:21:50 [ INFO]   float_t cash -> 3867.6003
//                    16:21:50 [ INFO]   float_t income -> 256.4
//                    16:21:50 [ INFO]   uint64_t eco_receipt_id -> 26
//                    16:21:50 [ INFO]   unknown_8 context -> 9
//                    16:21:50 [ INFO]   uint64_t subcontext -> 9
//                    16:21:50 [ INFO] Sending game event 0x71 (NetworkEvent::SNetRequestQueueBloonSetEvent) (Sim tick: 0x0, Unknown U8: 0x1)
//                    16:21:50 [ INFO]   uint8_t target_side -> 0
//                    16:21:50 [ INFO]   uint64_t sim_tick -> 0
//                    16:21:50 [ INFO]   uint32_t bloon_set_id -> 9
//                    16:21:50 [ INFO]   uint16_t bloon_features -> 0
//                    16:21:50 [ INFO]   uint32_t path_index -> 0
//                    16:21:50 [ INFO]   float_t cost -> 30
//                    16:21:50 [ INFO]   float_t income_change -> 1.8
//                    16:21:50 [ INFO]   uint64_t eco_receipt_id -> 2
                }

//                16:02:52 [ INFO] Send CMP 0x28368A66300 (length: 0x1EED)
//                16:02:54 [ INFO] Sending game event 0x85 (NetworkEvent::SNetEcoUpdatedEvent) (Sim tick: 0x9C3, Unknown U8: 0x0)
//                16:02:54 [ INFO]   float_t cash -> 930
//                16:02:54 [ INFO]   float_t income -> 251
//                16:02:54 [ INFO]   uint64_t eco_receipt_id -> 11
//                16:02:54 [ INFO]   unknown_8 context -> 9
//                16:02:54 [ INFO]   uint64_t subcontext -> 0
//                16:02:54 [ INFO] Sending game event 0x71 (NetworkEvent::SNetRequestQueueBloonSetEvent) (Sim tick: 0x0, Unknown U8: 0x0)
//                16:02:54 [ INFO]   uint8_t target_side -> 1
//                16:02:54 [ INFO]   uint64_t sim_tick -> 0
//                16:02:54 [ INFO]   uint32_t bloon_set_id -> 0
//                16:02:54 [ INFO]   uint16_t bloon_features -> 0
//                16:02:54 [ INFO]   uint32_t path_index -> 0
//                16:02:54 [ INFO]   float_t cost -> 15
//                16:02:54 [ INFO]   float_t income_change -> 1
//                16:02:54 [ INFO]   uint64_t eco_receipt_id -> 11
//                16:02:54 [ INFO]   uint8_t self_send_flag -> 0
            }

//            15:55:49 [ INFO] Sending game event 0x6D (NetworkEvent::SNetPlaceTowerEvent) (Sim tick: 0x6752, Unknown U8: 0x1)
//            15:55:49 [ INFO]   uint64_t sim_tick -> 26450
//            15:55:49 [ INFO]   uint32_t tower_type -> 0
//            15:55:49 [ INFO]   uint64_t opaque_id -> 7
//            15:55:49 [ INFO]   float_t pos_x -> 80.743225
//            15:55:49 [ INFO]   float_t pos_y -> -8.334209
//            15:55:49 [ INFO]   uint32_t layer -> 0
//            15:55:49 [ INFO]   float_t cost -> 200
//            15:55:49 [ INFO]   uint64_t tower_entity_id -> 1020009
//            15:55:49 [ INFO]   uint32_t tower_entity_id_version -> 1
//            15:55:49 [ INFO]   uint64_t parent_entity_id -> 0
//            15:55:49 [ INFO]   uint32_t parent_entity_id_version -> 0
//            15:55:49 [ INFO]   uint32_t upgrade_combo -> 50331648
//            15:55:49 [ INFO]   uint32_t preset_modifier_id_checksum -> 0
//            15:55:49 [ INFO]   uint32_t target_mode_override -> 0
//            15:55:49 [ INFO]   uint8_t reverse_flight_path -> 0
//            15:55:49 [ INFO]   uint64_t eco_receipt_id -> 100
        }
        ImGui::End();
    }

#define TRL(name, fmt, value_local, value_remote) \
do {                                                  \
    ImGui::TableNextRow(); \
    ImGui::TableSetColumnIndex(0); \
    ImGui::Text(name); \
    ImGui::TableSetColumnIndex(1); \
    ImGui::Text(fmt, value_local); \
    ImGui::TableSetColumnIndex(2); \
    ImGui::Text(fmt, value_remote);                   \
} while(0) \

    {
        ImGui::Begin("Net Debug");

        if(ImGui::BeginTable("cursor_values", 3)) {
            namespace debug = state::debug;

            {
                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Cursor");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text("Local");

                ImGui::TableSetColumnIndex(2);
                ImGui::Text("Remote");
            }

            TRL("  Pos X", "%f", debug::cursor_local.pos_x, debug::cursor_remote.pos_x);
            TRL("  Pos Y", "%f", debug::cursor_local.pos_y, debug::cursor_remote.pos_y);
            TRL("  Down ", "%s", debug::cursor_local.active ? "yes" : "no", debug::cursor_remote.active ? "yes" : "no");
            ImGui::EndTable();
        }

        ImGui::Dummy(ImVec2(0.0, 20.0));
        if(ImGui::BeginTable("tick_values", 3)) {
            namespace debug = state::debug;

            {
                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Network Ticks");

                ImGui::TableSetColumnIndex(1);
                ImGui::Text("Local");

                ImGui::TableSetColumnIndex(2);
                ImGui::Text("Remote");
            }

            TRL("  Net Tick", "%u", debug::network_tick_local.net_tick, debug::network_tick_remote.net_tick);
            TRL("  Sim Tick", "%llu", debug::network_tick_local.sim_tick, debug::network_tick_remote.sim_tick);

            TRL("  Latency", "%u", debug::network_tick_local.latency, debug::network_tick_remote.latency);
            TRL("  Jitter", "%u", debug::network_tick_local.jitter, debug::network_tick_remote.jitter);

            TRL("  Bloons created", "%llu", debug::network_tick_local.bloons_created, debug::network_tick_remote.bloons_created);
            TRL("  Bloons popped", "%llu", debug::network_tick_local.bloons_popped, debug::network_tick_remote.bloons_popped);
            TRL("  Bloons leaked", "%llu", debug::network_tick_local.bloons_leaked, debug::network_tick_remote.bloons_leaked);
            ImGui::EndTable();
        }

        ImGui::End();
    }
}