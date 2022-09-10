#pragma once

#include <cstdint>
#include <optional>
#include <string_view>

namespace mem {
    extern bool vm_write(uintptr_t /* address */, const void* /* buffer */, size_t /* size */);
    extern bool vm_read(uintptr_t /* address */, void* /* buffer */, size_t /* size */);

    // Find a pattern within the main exe handle.
    [[nodiscard]]
    extern std::optional<uintptr_t> find_pattern(const std::string_view& /* pattern */);

    [[nodiscard]]
    extern std::optional<uintptr_t> find_pattern_ex(uintptr_t /* module */, const std::string_view& /* pattern */);
}