//
// Created by WolverinDEV on 19/10/2022.
//

#include <memory>
#include "./http.h"
#include "../hook.h"
#include "../mem.h"
#include "../log.h"
#include "../util.h"

namespace hooks::http {
    std::shared_ptr<hook::JumpHook> hook_request{};
    std::shared_ptr<hook::JumpHook> hook_response{};

    bool initialize(std::string &error) {
        /* request hooking */
        {
            auto address_http_execute_request = mem::find_pattern("40 55 53 56 57 41 54 41 56 41 57 48 8D 6C 24 80 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 70 49 8B F9");
            if(!address_http_execute_request.has_value()) {
                logging::error("Failed to find http execute request function.");
                return false;
            }

            hook_request = hook::jump(*address_http_execute_request, 0x10, [](auto registers) {
                const auto& target_url = *(std::string*) registers->rcx;
                const auto& payload = *(std::string*) registers->r8;

                logging::info("HTTP request to {}\n{}", target_url, payload);
            });
        }

        /* Response hooking. */
        {
    #if 0
            /* Note: The offset only handles JSON results. The payload reference should be editable. */
            // 1.5.0 0x14515E6 + util::exe_offset
            // 1.5.1 90 48 8D 95 10 0B 00 00
            auto address_http_response = mem::find_pattern("90 48 8D 94 24 70 02 00 00 48 8D 8C 24 90 02 00 00 E8 ? ? ? ? 90 48 8B 9C 24 E0 02 00 00");
            if(!address_http_response) {
                logging::error("Failed to find http handle response function.");
                return false;
            }
            jump_hooks.push_back(hook::jump(*address_http_response, 0x0F, [](auto registers) {
                const auto& payload = *(std::string*) registers->rax;

                logging::info("HTTP response\n{}", payload);
            }));
    #endif
            // HTTP response handle method (unclean: only for json results?). rdx (arg2) contains the http response.
            auto address_http_handle_json = mem::find_pattern("48 89 5C 24 18 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 F0 EF FF FF");
            if(!address_http_handle_json) {
                logging::error("Failed to find http handle response json function.");
                return false;
            }

            auto address_http_get_payload_string = mem::find_pattern("40 53 48 83 EC ? 4C 8B 81 C8 00 00 00");
            if(!address_http_get_payload_string) {
                logging::error("Failed to find http response get payload string.");
                return false;
            }

            logging::info("Override method: {:x}", *address_http_handle_json - util::exe_offset);
            auto http_get_payload_string = (std::string*(*)(void*, std::string*)) *address_http_get_payload_string;
            hook_response = hook::jump(*address_http_handle_json, 0x10, [http_get_payload_string](auto registers) {
                logging::info("HTTP Response {:x}", registers->rdx);

                /* This copies the response therefor it's not editable. */
                std::string payload{};
                http_get_payload_string((void*) registers->rdx, &payload);

                logging::info("HTTP response\n{}", payload);
            });
        }
        return true;
    }

    void finalize() {
        hook_request = nullptr;
        hook_response = nullptr;
    }
}