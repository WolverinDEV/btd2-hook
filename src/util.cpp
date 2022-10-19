#include "./util.h"
#include "./log.h"
#include "./mem.h"
#include <regex>
#include <Windows.h>
#include <Psapi.h>

uintptr_t util::exe_offset = 0;
uintptr_t util::app_global = 0; // Is itself a ptr to app global!

bool util::initialize_offsets(std::string &error) {
    {
        auto handle_own_process = GetCurrentProcess();
        HMODULE base_address{};
        DWORD bytes_used{};
        if(!EnumProcessModules(handle_own_process, &base_address, sizeof(HMODULE), &bytes_used)) {
            error = "failed to get .exe base address.";
            return false;
        }

        util::exe_offset = (uintptr_t) base_address;
        logging::debug(".exe base offset: {:x}", util::exe_offset);
    }

    {
        // Try to find the app global variable by finding a reference in code.
        auto code_ptr = mem::find_pattern("48 8B 05 ? ? ? ? 48 85 C0 75 ? B9 ? ? ? ? E8 ? ? ? ? 48 8B D8 48 89 44 24 60 48 85 C0 74 ? 33 D2 41 B8 ? ? ? ? 48 8B C8 E8 ? ? ? ? 48 8B CB E8 ? ? ? ? EB ? 49 8B C5 48 89 05 ? ? ? ? 48 8B C8 E8 ? ? ? ? 48 8B C8");
        if(!code_ptr.has_value()) {
            error = "failed to find code referencing the app global variable.";
            return false;
        }

        auto offset = *(int32_t*) (*code_ptr + 3);
        util::app_global = *code_ptr + 7 + offset;
        logging::debug("App global: 0x{:X} (0x{:X})", util::app_global, util::app_global - util::exe_offset);
    }
    return true;
}

// 48 8B 05 ? ? ? ? 48 85 C0 75 ? B9 ? ? ? ? E8 ? ? ? ? 48 8B D8 48 89 44 24 60 48 85 C0 74 ? 33 D2 41 B8 ? ? ? ? 48 8B C8 E8 ? ? ? ? 48 8B CB E8 ? ? ? ? EB ? 49 8B C5 48 89 05 ? ? ? ? 48 8B C8 E8 ? ? ? ? 48 8B C8

std::string util::decode_xor_string(uint64_t key, uintptr_t function_ptr, const std::optional<size_t> &length) {
    MEMORY_BASIC_INFORMATION mbi{};
    if(!VirtualQueryEx(GetCurrentProcess(), (void*) function_ptr, &mbi, sizeof(mbi))) {
        logging::warn("Failed to query address memory info: {}", logging::last_error_message());
        return "";
    }

    if(mbi.State != MEM_COMMIT) {
        return "-- invalid address --";
    }

    if(!(mbi.Protect & (PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE))) {
        /* target is not executable */
        return "-- not a function --";
    }

    auto fn_encrypted_data = (void*(*)()) function_ptr;
    auto encrypted_data = fn_encrypted_data();
    if(!VirtualQueryEx(GetCurrentProcess(), (void*) encrypted_data, &mbi, sizeof(mbi))) {
        logging::warn("Failed to query address memory info: {}", logging::last_error_message());
        return "";
    }
    if(mbi.State != MEM_COMMIT) {
        return "-- function returned invalid address --";
    }

    auto encrypted_data_ptr = (uint8_t*) encrypted_data;

    // Test if the string has already been decrypted
    for(size_t index{0}; index < length.value_or(0xFFFF); index++) {
        if(*encrypted_data_ptr == 0) {
            return std::string{(const char*) encrypted_data};
        } else if(!isprint(*encrypted_data_ptr)) {
            break;
        }
    }

    std::string buffer{};
    buffer.reserve(length.value_or(1024));

    for(size_t index{0}; index < length.value_or(0xFFFF); index++) {
        auto encrypted_value = *(encrypted_data_ptr++);
        auto value = encrypted_value ^ (uint8_t) (key >> (8 * (index & 7)));
        if(!value && !length.has_value()) {
            break;
        }

        buffer.push_back((char) value);
    }

    return buffer;
}

// The HTTP Sign arg is constant access versions: B42D1230A78185FF
// At the position where we find this, there is also another string.
// It's currently unclear for what this is for.
std::string util::get_http_sign_arg2() {
#if 0 \
    /* Version 1.5.1 */
    auto address = mem::find_pattern("E8 ? ? ? ? 48 8B C8 E8 ? ? ? ? 48 8B D8 48 8B 8E B8 00 00 00");
#else
    /* Version 1.6.1 */
    auto address = mem::find_pattern("E8 ? ? ? ? 48 8B C8 E8 ? ? ? ? 48 8B D8 48 8B 8E B0 00 00 00");
#endif
    if(!address.has_value()) {
        logging::error("Failed to find pattern for http sign arg2");
        return "";
    }

    auto get_value = util::decode_call_target(*address);
    if(!get_value.has_value()) {
        logging::error("failed to find decode call for get_value.");
        return "";
    }

    auto decode_value = util::decode_call_target(*address + 5 + 3);
    if(!decode_value.has_value()) {
        logging::error("failed to find decode call for decode_value.");
        return "";
    }

    auto fn_get_value = (char*(*)()) *get_value;
    auto fn_decode_value = (char*(*)(char*)) *decode_value;

    auto value = fn_get_value();
    fn_decode_value(value);

    return std::string{value};
}

std::string util::get_http_auth_device() {
    // Get the steam device id: 48 89 5C 24 10 48 89 74 24 18 48 89 7C 24 20 55 41 54 41 55 41 56 41 57 48 8D 6C 24 90 48 81 EC ? ? ? ? 4C 8B F1
    // Get the actual device value: 48 89 5C 24 08 48 89 6C 24 18 48 89 74 24 20 57 41 56 41 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 70
    auto address = mem::find_pattern("48 89 5C 24 08 48 89 6C 24 18 48 89 74 24 20 57 41 56 41 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 70");
    if(!address.has_value()) {
        logging::error("Failed to find pattern for http device id");
        return "";
    }

    auto fn_get_value = (std::string*(*)(void*, std::string*)) *address;

    std::string result{};
    fn_get_value(nullptr, &result);
    return result;
}

std::string util::get_http_auth_session() {
    auto address_get_session = mem::find_pattern("40 53 48 83 EC ? 48 8B D9 48 8B 01 48 85 C0 75 ?");
    if(!address_get_session.has_value()) {
        logging::error("Failed to find pattern for get session from app global.");
        return "";
    }

    auto address_get_session_id = mem::find_pattern("48 8B 01 48 05 ? ? ? ? C3 CC CC CC CC CC CC 40 53 48 83 EC ?");
    if(!address_get_session_id.has_value()) {
        logging::error("Failed to find pattern for get session id.");
        return "";
    }

    auto fn_get_session = (void*(*)(void* /* app global */)) *address_get_session;
    auto fn_get_session_id = (std::string*(*)(void* /* session */)) *address_get_session_id;

    auto address_session = fn_get_session(*(void**) util::app_global);
    auto target = fn_get_session_id(address_session);
    return *target;
}

std::optional<uintptr_t> util::decode_call_target(uintptr_t op_address) {
    auto op_ptr = (uint8_t*) op_address;

    switch(*op_ptr) {
        case 0xE8: // Relative call
            return std::make_optional(op_address + 5 + *(int32_t*) (++op_ptr));

        default:
            logging::warn("Tried to decode call target at 0x{:X} but opcode isn't a call ({:X}).", op_address, *op_ptr);
            return std::nullopt;
    }
}

std::string util::replace_all(const std::string_view &text, std::initializer_list<std::pair<std::string, std::string>> values) {
    std::string result{text};
    for (const auto& pair : values)
    {
        std::regex replace{pair.first};
        result = regex_replace(result, replace, pair.second);
    }
    return result;
}