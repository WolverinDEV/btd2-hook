#include "./mem.h"
#include "./log.h"
#include "./util.h"
#include <vector>
#include <Windows.h>
#include <Psapi.h>

bool mem::vm_read(uintptr_t address, void *buffer, size_t length) {
    auto hProcess = GetCurrentProcess();
    return ReadProcessMemory(hProcess, (void*) address, buffer, length, nullptr);
}

bool mem::vm_write(uintptr_t address, const void *buffer, size_t length) {
    DWORD dwOldProtect;
    auto hProcess = GetCurrentProcess();
    if(!VirtualProtectEx(hProcess, (void*) address, length, PAGE_EXECUTE_READWRITE, &dwOldProtect)) {
        logging::warn("Failed unprotect for write {} bytes at {:X}: {}", length, address, logging::last_error_message());
        return false;
    }

    SIZE_T bytes_written{};
    auto result = WriteProcessMemory(hProcess, (void*) address, buffer, length, &bytes_written);
    if(!VirtualProtectEx(hProcess, (void*) address, length, dwOldProtect, nullptr)) {
        //logging::warn("Failed to restore old protection for bytes written at 0x{:X}: {}", address, logging::last_error_message());
    }

//    if(result) {
//        logging::debug("0x{:X} Wrote {}/{} bytes.", address, bytes_written, length);
//    } else {
//        logging::debug("0x{:X} Failed to write {} bytes.", address, length);
//    }
    return result;
}

inline bool parse_search_pattern(const std::string_view &pattern, std::vector<uint8_t>& bytes, std::vector<bool>& mask) {
    size_t index{0};
    do {
        auto end_index = pattern.find(' ', index);
        auto sval = pattern.substr(index, end_index - index);
        if(sval.find_first_not_of('?') == std::string::npos) {
            bytes.push_back(0);
            mask.push_back(false);
        } else {
            try {
                auto value = std::stoull(std::string{sval}, nullptr, 16);
                bytes.push_back(value);
                mask.push_back(true);
            } catch(std::exception&) {
                return false;
            }
        }

        index = end_index + 1;
    } while(index);

    return true;
}

std::optional<uintptr_t> mem::find_pattern(const std::string_view &pattern) {
    return mem::find_pattern_ex(util::exe_offset, pattern);
}

std::optional<uintptr_t> mem::find_pattern_ex(uintptr_t handle_module, const std::string_view &pattern) {
    std::vector<uint8_t> bytes{};
    std::vector<bool> mask{};
    if(!parse_search_pattern(pattern, bytes, mask)) {
        logging::warn("Can not parse pattern: {}", pattern);
        return std::nullopt;
    }

    MODULEINFO module_info{};
    {
        auto success = GetModuleInformation(
                GetCurrentProcess(),
                (HMODULE) handle_module,
                &module_info,
                sizeof(module_info)
        );
        if(!success) {
            logging::warn("Failed to query module info for 0x{:X}", handle_module);
            return std::nullopt;
        }

        if(module_info.SizeOfImage < bytes.size()) {
            logging::warn("Module is smaller than supplied pattern (0x{:X} < 0x{:X}). This should not happen.", module_info.SizeOfImage, bytes.size());
            return std::nullopt;
        }
    }

    auto current_address = (uint8_t*) handle_module;
    const auto address_end = current_address + module_info.SizeOfImage - bytes.size();
    do {
        size_t index{0};
        for(; index < bytes.size(); index++) {
            if(!mask[index]) {
                continue;
            }

            uint8_t value = *(current_address + index);
            if(value != bytes[index]) {
                break;
            }

            /* byte match */
        }

        if(index == bytes.size()) {
            // pattern found!
            return std::make_optional((uintptr_t) current_address);
        }

    } while(++current_address < address_end);
    return std::nullopt;
}