#include "./log.h"
#include "./util.h"
#include "hooks/reflang.h"
#include "./game_event.h"
#include "./ui.h"
#include "./hooks/http.h"
#include "./hooks/protocol.h"
#include <Windows.h>

bool initialize();
void finalize();

[[maybe_unused]] BOOL WINAPI DllMain([[maybe_unused]] HINSTANCE handle_dll, DWORD call_reason, LPVOID ptr_reserved) {
    switch( call_reason ) {
        case DLL_PROCESS_ATTACH:
            return initialize();

        case DLL_PROCESS_DETACH:
            if(ptr_reserved) {
                /* Parent exe is terminating. */
                return true;
            }

            finalize();
            return true;

        default:
            /* Nothing to handle. */
            return true;
    }
}

struct Hook {
    const char* name;
    bool(*fn_initialize)(std::string&);
    void(*fn_finalize)();
};

static Hook registered_hooks[] = {
    Hook{ "HTTP", hooks::http::initialize, hooks::http::finalize },
    Hook{ "protocol", hooks::protocol::initialize, hooks::protocol::finalize },
    // Reflang is a helper for dumping all registered GameEvents (This was originally created for version 1.5.0)
    // Hook{ "reflang", hooks::reflang::initialize, hooks::reflang::finalize },
};

bool initialize() {
    std::string error{};
    if(!logging::initialize(error)) {
        auto message = "Failed to initialize logging: \n" + error;
        MessageBox(nullptr, message.c_str(), "Log init failed", MB_OK);
        return false;
    }

    if(!util::initialize_offsets(error)) {
        logging::error("Failed to initialize offsets:\n{}", error);
        return false;
    }

    if(!ui::initialize(error)) {
        logging::error("Failed to initialize UI:\n{}", error);
        return false;
    }

    logging::info("Device ID: {}", util::get_http_auth_device());
    logging::info("Session ID: {}", util::get_http_auth_session());
    logging::info("Http arg 2: {}", util::get_http_sign_arg2());

    for(const auto& hook : registered_hooks) {
        if(!hook.fn_initialize(error)) {
            logging::error("Failed to initialize hook {}:\n{}", hook.name, error);
            return false;
        }
    }

    logging::info("Inject success!");
    return true;
}

void finalize() {
    ui::finalize();
    for(const auto& hook : registered_hooks) {
        hook.fn_finalize();
    }
    logging::info("Shutting down succeeded.");
}