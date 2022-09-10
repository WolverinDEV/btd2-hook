#include "./ui.h"
#include "./hook.h"
#include "util.h"
#include "game_event.h"
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

extern void send_game_event(game_event::GameEvent* /* event */);

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
        ImGui::End();
    }
}