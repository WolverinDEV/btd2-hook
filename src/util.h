#pragma once

#include <cstdint>
#include <string>
#include <optional>

namespace util {
    extern uintptr_t exe_offset;
    extern uintptr_t app_global;

    [[nodiscard]]
    extern std::string decode_xor_string(uint64_t /* key */, uintptr_t /* function ptr */, const std::optional<size_t>& /* length */ = std::nullopt);

    extern bool initialize_offsets(std::string& /* error */);

    [[nodiscard]]
    extern std::string get_http_sign_arg2();

    [[nodiscard]]
    extern std::string get_http_auth_device();

    [[nodiscard]]
    extern std::string get_http_auth_session();

    [[nodiscard]]
    extern std::optional<uintptr_t> decode_call_target(uintptr_t /* call op code */);

    [[nodiscard]]
    extern std::string replace_all(const std::string_view& /* text */, std::initializer_list<std::pair<std::string, std::string>> /* values */);
}