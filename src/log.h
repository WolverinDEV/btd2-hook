#pragma once

#include <string>
#include <fmt/core.h>

namespace logging {
    extern bool initialize(std::string& /* error */);
    extern void finalize();

    extern void message(const std::string_view& /* level */, const std::string_view& /* formatted message */);

    extern std::string last_error_message();

    inline auto hex_dump(void* target, size_t length) -> std::string {
        auto bptr = (uint8_t*) target;
        std::string buffer{};
        buffer.reserve(length * 3);
        for(size_t index{0}; index < length; index++) {
            buffer.append(fmt::format("{:02X} ", *(bptr++)));
        }

        buffer.resize(buffer.size() - 1);
        return buffer;
    }
}

#define LOGGING_LEVEL_FUNCTIONS(function_name, level_prefix)                                \
namespace logging {                                                                         \
    template<typename ...T>                                                                 \
    inline void function_name(fmt::format_string<T...> fmt, T&&... args) {                  \
        try {                                                                               \
            auto message = ::fmt::format(fmt, args...);                                     \
            ::logging::message(level_prefix, message);                                      \
        } catch(...) {                                                                      \
            ::logging::message("ERROR", ::fmt::format("Failed to write logging message: {}", fmt));        \
        }                                                                                   \
    }                                                                                       \
}

LOGGING_LEVEL_FUNCTIONS(info,  " INFO")
LOGGING_LEVEL_FUNCTIONS(debug, "DEBUG")
LOGGING_LEVEL_FUNCTIONS(trace, "TRACE")
LOGGING_LEVEL_FUNCTIONS(warn,  " WARN")
LOGGING_LEVEL_FUNCTIONS(error, "ERROR")