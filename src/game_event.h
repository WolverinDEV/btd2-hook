#pragma once

#include <functional>
#include <string>

#include <fmt/core.h>
#include <memory>
#include <map>
#include <array>
#include <fmt/format.h>

#include "./payload_buffer.h"

namespace game_event {
    class GameEvent {
        public:
            using iter_fields_callback_t = std::function<void(const std::string & /* name */,
                                                              const std::string & /* type */,
                                                              const std::string & /* value */)>;

            explicit GameEvent(uint16_t event_id, uint64_t sim_tick, uint8_t player_id)
                    : event_id{event_id}, sim_tick{sim_tick}, player_id{player_id} {}

            virtual ~GameEvent() = default;

            [[nodiscard]] virtual std::string event_name() const = 0;

            virtual void iter_fields(const iter_fields_callback_t & /* callback */) const = 0;

            virtual bool encode(NetworkMessageBuffer& /* writer */) const = 0;
        protected:
            uint16_t event_id;
            uint64_t sim_tick;
            uint8_t player_id;
    };

    typedef std::array<uint64_t, 2> uint128_t;
    struct SNetAssetSignature { };
    struct SNetPrimingOutputStep { };

}
namespace game_event {
    typedef uint8_t unknown_8;
    typedef uint16_t unknown_16;
    typedef uint32_t unknown_32;
    #include "./game_event_generated.h"
}

template <> struct fmt::formatter<game_event::SNetAssetSignature> {
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
        auto it = ctx.begin(), end = ctx.end();
        if (it != end && *it != '}') throw fmt::format_error("invalid format");
        return it;
    }

    template <typename FormatContext>
    auto format(const game_event::SNetAssetSignature& p, FormatContext& ctx) const -> decltype(ctx.out()) {
        return fmt::format_to(ctx.out(), "<SNetAssetSignature>");
    }
};
template <> struct fmt::formatter<game_event::SNetPrimingOutputStep> {
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
        auto it = ctx.begin(), end = ctx.end();
        if (it != end && *it != '}') throw fmt::format_error("invalid format");
        return it;
    }

    template <typename FormatContext>
    auto format(const game_event::SNetAssetSignature& p, FormatContext& ctx) const -> decltype(ctx.out()) {
        return fmt::format_to(ctx.out(), "<SNetAssetSignature>");
    }
};
template <> struct fmt::formatter<game_event::uint128_t> {
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
        auto it = ctx.begin(), end = ctx.end();
        if (it != end && *it != '}') throw fmt::format_error("invalid format");
        return it;
    }

    template <typename FormatContext>
    auto format(const game_event::uint128_t& p, FormatContext& ctx) const -> decltype(ctx.out()) {
        return fmt::format_to(ctx.out(), "0x{:X} 0x{:X}", p[0], p[1]);
    }
};