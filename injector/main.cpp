#include <string>
#include <string_view>
#include <iostream>
#include <optional>
#include <array>
#include <algorithm>
#include <memory>

#include <Windows.h>
#include <tlhelp32.h>
#include <Psapi.h>

void error_exit(const std::string_view &message) {
    std::cout << "A critical error occurred:\n";
    std::cout << message << "\n";

    auto last_error = ::GetLastError();
    if(last_error > 0) {
        LPSTR messageBuffer = nullptr;
        size_t size = FormatMessageA(
                FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                nullptr, last_error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR) &messageBuffer, 0, nullptr);
        std::cout << "\n";
        std::cout << "Last windows error: " << std::hex << last_error << "\n";
        std::cout << "           Message: " << std::string_view{messageBuffer, size} << std::endl;
        LocalFree(messageBuffer);
    }
}

bool is_compatibile(HANDLE hProcess) {
    BOOL isTargetWow64{false};
    IsWow64Process(hProcess, &isTargetWow64);

    BOOL isInjectorWow64{false};
    IsWow64Process(GetCurrentProcess(), &isInjectorWow64);

    return isTargetWow64 == isInjectorWow64;
}

DWORD FindProcessId(const std::string &processName) {
    PROCESSENTRY32 processInfo;
    processInfo.dwSize = sizeof(processInfo);

    HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (processesSnapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    if (!Process32First(processesSnapshot, &processInfo)) {
        return 0;
    }

    do {
        if (processName == processInfo.szExeFile) {
            CloseHandle(processesSnapshot);
            return processInfo.th32ProcessID;
        }
    } while (Process32Next(processesSnapshot, &processInfo));


    CloseHandle(processesSnapshot);
    return 0;
}

std::optional<HANDLE> process_find_module_by_name(HANDLE handle_process, std::string target_module_name) {
    std::array<HMODULE, 0x1FF> modules{};
    DWORD module_count{};
    if (!EnumProcessModules(handle_process, modules.data(), (DWORD) modules.size() * sizeof(HMODULE), &module_count)) {
        if(GetLastError() == ERROR_PARTIAL_COPY) {
            error_exit("Injector seems to be 32 bit but target is 64 bit process.");
        }

        error_exit("Failed to iterate loaded modules.");
        return std::nullopt;
    }

    std::transform(target_module_name.begin(), target_module_name.end(), target_module_name.begin(), std::tolower);
    for (size_t index{0}; index < module_count / sizeof(HMODULE); index++) {
        std::string module_name{};
        module_name.resize(1024);
        module_name.resize(
            GetModuleFileNameExA(handle_process, modules[index], module_name.data(), (DWORD) module_name.size())
        );

        if (module_name.empty()) {
            // An error occurred
            continue;
        }

        std::transform(module_name.begin(), module_name.end(), module_name.begin(), std::tolower);
        //std::cout << " " << module_name << "\n";
        if(target_module_name == module_name) {
            return modules[index];
        }
    }

    return std::nullopt;
}

std::shared_ptr<void> process_write_memory(HANDLE hProcess, const void* buffer, size_t buffer_size, DWORD protect) {
    auto address = VirtualAllocEx(hProcess, nullptr, buffer_size, MEM_COMMIT | MEM_RESERVE, protect);
    if (!address) {
        return nullptr;
    }

    if (!WriteProcessMemory(hProcess, address, buffer, buffer_size, nullptr)) {
        VirtualFreeEx(hProcess, address, buffer_size, MEM_FREE);
        return nullptr;
    }

    return std::shared_ptr<void>{
        (void*) address,
        [hProcess, address](void* ptr) {
            VirtualFreeEx(hProcess, address, 0, MEM_RELEASE);
        }
    };
}

void inject_dll(HANDLE handle_process, const std::string& target_dll) {
    HMODULE hModule = GetModuleHandleW(L"kernel32.dll");
    if (!hModule) {
        error_exit("failed to locate kernel32.dll");
        return;
    }

    FARPROC hLoadLib = GetProcAddress(hModule, "LoadLibraryA");
    if (!hLoadLib) {
        error_exit("failed to locate function LoadLibraryA");
        return;
    }

    // size() + 1 since we want to copy the null terminator as well.
    auto remote_ptr = process_write_memory(handle_process, target_dll.c_str(), target_dll.size() + 1, PAGE_READWRITE);
    if (!remote_ptr) {
        error_exit("failed to write file path into remote process");
        return;
    }

    // Inject to the remote process:
    HANDLE handle_thread = CreateRemoteThread(handle_process, nullptr, 0, (LPTHREAD_START_ROUTINE) hLoadLib, remote_ptr.get(), NULL, nullptr);
    if(!handle_thread) {
        error_exit("Failed to create remote thread.");
        return;
    }

    auto wait_result = WaitForSingleObject(handle_thread, 5000);
    if(wait_result != WAIT_OBJECT_0) {
        error_exit("failed to inject dll within 5000ms");
        return;
    }

    DWORD exit_code{0xFFFF};
    GetExitCodeThread(handle_thread, &exit_code);
    std::cout << "Exit code: " << exit_code << "\n";
    CloseHandle(handle_thread);
}

void process_eject_dll(HANDLE handle_process, HANDLE target_dll_handle) {
    HMODULE hModule = GetModuleHandleW(L"kernel32.dll");
    if (!hModule) {
        error_exit("failed to locate kernel32.dll");
        return;
    }

    FARPROC handle_free_library = GetProcAddress(hModule, "FreeLibrary");
    if (!handle_free_library) {
        error_exit("failed to locate function FreeLibrary");
        return;
    }

    HANDLE handle_thread = CreateRemoteThread(
            handle_process, nullptr,
            0, (LPTHREAD_START_ROUTINE) handle_free_library,
            target_dll_handle, NULL, nullptr
    );
    if(!handle_thread) {
        error_exit("Failed to create remote thread.");
        return;
    }

    auto wait_result = WaitForSingleObject(handle_thread, 5000);
    if(wait_result != WAIT_OBJECT_0) {
        error_exit("failed to eject dll within 5000ms");
        return;
    }

    DWORD exit_code{0xFFFF};
    GetExitCodeThread(handle_thread, &exit_code);
    std::cout << "Exit code: " << exit_code << "\n";
    CloseHandle(handle_thread);
}

enum struct Action {
    Inject,
    Eject,
    Test
};

int main(int argc, char** argv) {
    if(argc != 3) {
        error_exit("Invalid argument count.\n<inject|eject|test> <target dll>");
        return 1;
    }

    Action target_action;
    if(!strcmp(argv[1], "inject")) {
        target_action = Action::Inject;
    } else if(!strcmp(argv[1], "eject")) {
        target_action = Action::Eject;
    } else if(!strcmp(argv[1], "test")) {
        target_action = Action::Test;
    } else {
        error_exit("Invalid action.");
        return 1;
    }

    std::string target_dll{argv[2]};

    auto process_id{FindProcessId("btdb2_game.exe")};
    if (!process_id) {
        error_exit("Failed to locate btd battles2 process. Is the game running?");
        return 1;
    }

    std::cout << "BTD Battles 2 process id: " << process_id << "\n";

    HANDLE hProcess = OpenProcess(
            PROCESS_CREATE_THREAD | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_QUERY_INFORMATION,
            FALSE,
           process_id
    );
    if(!hProcess) {
        error_exit("Failed to open target process");
        return 1;
    }

    if(!is_compatibile(hProcess)) {
        error_exit("Injector is not compatible with target process.");
        return 1;
    }

    auto dll_handle = process_find_module_by_name(hProcess, target_dll);
    switch(target_action) {
        case Action::Inject:
            if(dll_handle) {
                error_exit("DLL already injected.");
                return 1;
            }

            std::cout << "Injecting DLL " << target_dll << "\n";
            inject_dll(hProcess, target_dll);

            dll_handle = process_find_module_by_name(hProcess, target_dll);
            if(!dll_handle) {
                error_exit("Failed to find injected DLL. Inject failed.");
                return 1;
            }

            std::cout << "Successfully injected DLL.\n";
            return 0;

        case Action::Eject:
            if(!dll_handle) {
                error_exit("DLL not injected.");
                return 1;
            }

            std::cout << "Ejecting DLL " << target_dll << "\n";
            process_eject_dll(hProcess, *dll_handle);

            dll_handle = process_find_module_by_name(hProcess, target_dll);
            if(dll_handle) {
                error_exit("Failed to find eject DLL. Inject failed.");
                return 1;
            }

            std::cout << "Successfully ejected DLL.\n";
            return 0;

        case Action::Test:
            if(dll_handle) {
                std::cout << "DLL injected.\n";
            } else {
                std::cout << "DLL not injected.\n";
            }
            return 0;
    }
}
