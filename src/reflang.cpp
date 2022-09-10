#include "reflang.h"
#include "util.h"
#include "log.h"
#include "./payload_buffer.h"
#include <unordered_map>
#include <memory>
#include <fstream>
#include <map>
#include <sstream>
#include <deque>
#include <regex>

// Reflang reference:
// https://github.com/chakaz/reflang/blob/master/lib/reflang.hpp
//
// Note:
// The actual return values of the V-Table is different
// from the one in the header. My guess it has been modified.
namespace reflang {
    using type_t = void*; // Types are ptrs which point to just zeros.

    struct Reference {
        type_t type;
        [[maybe_unused]] void* data_ptr;
    };

    class IType {
        public:
            virtual ~IType() = 0;
            [[nodiscard]] virtual std::string& get_name() const = 0;
    };

    class IClass : public IType {
        public:
            [[nodiscard]] virtual type_t get_type() const = 0;
            [[maybe_unused]] virtual void unknown_18() const = 0;

            [[nodiscard]] virtual uint32_t get_field_count() const = 0;
            [[nodiscard]] virtual std::vector<std::string>& get_field_list() const = 0;

            [[maybe_unused]] virtual void unknown_30() const = 0;
            [[maybe_unused]] virtual void unknown_38() const = 0;

            virtual Reference* get_field(Reference& /* result */, const Reference& /* instance */, const std::string& /* name */) const = 0;

            [[maybe_unused]] virtual void unknown_48() const = 0; // Returns a number again.
            [[maybe_unused]] virtual void unknown_50() const = 0;
            [[maybe_unused]] virtual void unknown_58() const = 0;
            [[maybe_unused]] virtual void unknown_60() const = 0;
    };
}

using encoder_fn_t = void(*)(reflang::Reference*, void* /* payload wrapper */);
using decoder_fn_t = void(*)(void* /* payload wrapper */, reflang::Reference*);

std::map<uint32_t, std::string> game_event_mapping{
        { 0x1, "NetworkEvent::SNetInitPlayerRequestEvent" },
        { 0x2, "NetworkEvent::SNetInitPlayerResponseEvent" },
        { 0x3, "NetworkEvent::SNetInitPlayerPostponedEvent" },
        { 0x4, "NetworkEvent::SNetInitPlayerCompleteEvent" },
        { 0x5, "NetworkEvent::SNetStartLobbyEvent" },
        { 0x6, "NetworkEvent::SNetLobbyLoadedEvent" },
        { 0x7, "NetworkEvent::SNetLobbyErrorEvent" },
        { 0x9, "NetworkEvent::SNetTowerLoadoutEvent" },
        { 0xA, "NetworkEvent::SNetBlingLoadoutEvent" },
        { 0xB, "NetworkEvent::SNetSetGameMapEvent" },
        { 0xC, "NetworkEvent::SNetReadyUpEvent" },
        { 0xD, "NetworkEvent::SNetStartLoadingEvent" },
        { 0xE, "NetworkEvent::SNetGameLoadedEvent" },
        { 0xF, "NetworkEvent::SNetStartGameEvent" },
        { 0x10, "NetworkEvent::SNetRequestMapSkipEvent" },
        { 0x11, "NetworkEvent::SNetTriggerMapSkipEvent" },
        { 0x12, "NetworkEvent::SNetMoveToTowerSelectEvent" },
        { 0x13, "NetworkEvent::SNetRefereeSelectedTowerLoadoutEvent" },
        { 0x64, "NetworkEvent::SNetReadyToPlayEvent" },
        { 0x65, "NetworkEvent::SNetTickEvent" },
        { 0x66, "NetworkEvent::SNetScheduleRoundStartEvent" },
        { 0x67, "NetworkEvent::SNetConfirmRoundStartEvent" },
        { 0x68, "NetworkEvent::SNetSendLifeChangedEvent" },
        { 0x69, "NetworkEvent::SNetSendCriticalLifeLostEvent" },
        { 0x6A, "NetworkEvent::SNetRequestCurrentHealthEvent" },
        { 0x6B, "NetworkEvent::SNetSendFirstBloodEvent" },
        { 0x6C, "NetworkEvent::SNetGameResultEvent" },
        { 0x6D, "NetworkEvent::SNetPlaceTowerEvent" },
        { 0x6E, "NetworkEvent::SNetUpgradeTowerEvent" },
        { 0x6F, "NetworkEvent::SNetUpgradeTowerEvent" },
        { 0x70, "NetworkEvent::SNetChangeTargetModeTowerEvent" },
        { 0x71, "NetworkEvent::SNetRequestQueueBloonSetEvent" },
        { 0x72, "NetworkEvent::SNetQueueBloonSetEvent" },
        { 0x73, "NetworkEvent::SNetCollectablesCollectedEvent" },
        { 0x74, "NetworkEvent::SNetPlayerShieldActivatedEvent" },
        { 0x75, "NetworkEvent::SNetRequestFireWeaponEvent" },
        { 0x76, "NetworkEvent::SNetBombBlitzActivatedEvent" },
        { 0x77, "NetworkEvent::SNetRequestStartTowerFlightPathRejoinEvent" },
        { 0x78, "NetworkEvent::SNetRequestSetTowerTargetPointEvent" },
        { 0x79, "NetworkEvent::SNetRemoveBlockerEvent" },
        { 0x7A, "NetworkEvent::SNetSetNameEvent" },
        { 0x7B, "NetworkEvent::SNetRequestFirePlayerAbilityEvent" },
        { 0x7C, "NetworkEvent::SNetFirePlayerAbilityEvent" },
        { 0x7D, "NetworkEvent::SNetSendSurrenderNetEvent" },
        { 0x7E, "NetworkEvent::SNetShowEmoteEvent" },
        { 0x7F, "NetworkEvent::SNetRequestSimSnapshotEvent" },
        { 0x80, "NetworkEvent::SNetSimSnapshotEvent" },
        { 0x81, "NetworkEvent::SNetRequestSendTauntEvent" },
        { 0x82, "NetworkEvent::SNetSetBadgesEvent" },
        { 0x83, "NetworkEvent::SNetLastRoundGameEndRequestEvent" },
        { 0x84, "NetworkEvent::SNetTargetTouchPointEvent" },
        { 0x85, "NetworkEvent::SNetEcoUpdatedEvent" },
        { 0x86, "NetworkEvent::SNetPatrolPointsSelectedEvent" },
        { 0x87, "NetworkEvent::SNetLockInPlacePointSelectedEvent" },
        { 0x88, "NetworkEvent::SNetCenterPointSelectedEvent" },
        { 0x89, "NetworkEvent::SNetRequestSwitchHandEvent" },
        { 0x8A, "NetworkEvent::SNetRequestSwitchFlyingDirectionEvent" },
        { 0x8B, "NetworkEvent::SNetAssetBlobUploadEvent" },
        { 0x8C, "NetworkEvent::SNetDidQueueBloonSetEvent" },
        { 0x8D, "NetworkEvent::SNetDidFirePlayerAbilityEvent" },
        { 0x8E, "NetworkEvent::SNetRequestWithdrawCashFromTowerEvent" },
};

// Event ID vtable mapping (used for generating game_event_mapping).
// These can be decoded from the decoder/encoder functions for game events.
// They usually contain huge switch blocks containing all ids.
std::map<uint32_t, uintptr_t> game_event_mapping_vtable1{
        {0x01, 0x18F4940},
        {0x02, 0x18F0128},
        {0x03, 0x18F48D8},
        {0x04, 0x18F4870},
        {0x05, 0x18F4DB8},
        {0x06, 0x18F0190},
        {0x07, 0x18F49A8},
        {0x09, 0x18F0400},
        {0x0A, 0x18F0058},
        {0x0B, 0x18F4C80},
        {0x0C, 0x18F01F8},
        {0x0D, 0x18F4D50},
        {0x0E, 0x18F00C0},
        {0x0F, 0x18F4CE8},
        {0x10, 0x18F0260},
        {0x11, 0x18F4E20},
        {0x12, 0x18F4A10},
        {0x13, 0x18F4AE0},

        {0x64, 0x18F4A78},
        {0x65, 0x18A28A8},
        {0x66, 0x18A26A0},
        {0x67, 0x18F4600},
        {0x68, 0x18A2770},
        {0x69, 0x18A2708},
        {0x6A, 0x18F4B48},
        {0x6B, 0x18F4C18},
        {0x6C, 0x18F4808},
        {0x6D, 0x18A1D08},
        {0x6E, 0x18A21E8},
        {0x6F, 0x18A21E8},
        {0x70, 0x18A1B68},
        {0x71, 0x18A2638},
        {0x72, 0x18A1DD8},
        {0x73, 0x18A2C78},
        {0x74, 0x18A1D70},
        {0x75, 0x18A1F10},
        {0x76, 0x18A1A98},
        {0x77, 0x18A1FE0},
        {0x78, 0x18A1F78},
        {0x79, 0x18A1E40},
        {0x7A, 0x18F0398},
        {0x7B, 0x18A25D0},
        {0x7C, 0x18A1BD0},
        {0x7D, 0x18A27D8},
        {0x7E, 0x18A2840},
        {0x7F, 0x18F4BB0},
        {0x80, 0x18A0860},
        {0x81, 0x18F02C8},
        {0x82, 0x18F0330},
        {0x83, 0x18A2568},
        {0x84, 0x18A2180},
        {0x85, 0x18A2500},
        {0x86, 0x18A1CA0},
        {0x87, 0x18A1C38},
        {0x88, 0x18A1B00},
        {0x89, 0x18A20B0},
        {0x8A, 0x18A2048},
        {0x8B, 0x18EFFF0},
        {0x8C, 0x18A2498},
        {0x8D, 0x18F47A0},
        {0x8E, 0x18A2118}
};

// Initial type mapping.
// Will later be done via known field types so the type addresses can change.
std::unordered_map<uintptr_t, std::string> type_mapping{
        // Signed/unsigned order is guessed, but likely (derived from field names).
        {0x18D12F8, "uint8_t"},
        {0x18D9D10, "std::vector<uint8_t>"}, // Without length restriction
        {0x18F3A80, "uint16_t"},
        {0x18F4F50, "std::vector<uint16_t>"}, // Without length restriction
        {0x189DE50, "uint32_t"},
        {0x18D1240, "std::vector<uint32_t>"}, // Without length restriction
        {0x189DE20, "uint64_t"},
        {0x18F4F58, "std::vector<uint64_t>"}, // Without length restriction

        {0x18D9CA8, "int8_t"},
        {0x18F4F60, "std::vector<int8_t>"}, // Without length restriction
        {0x18F4F68, "int16_t"},
        {0x18F4F70, "std::vector<int16_t>"}, // Without length restriction
        {0x189DE58, "int32_t"},
        {0x18F4F78, "std::vector<int32_t>"}, // Without length restriction
        {0x189DE60, "int64_t"},
        {0x18F4F80, "std::vector<int64_t>"}, // Without length restriction

        {0x189F2A0, "float_t"},
        {0x18F4F88, "std::vector<float_t>"}, // Without length restriction
        {0x18F4F90, "double_t"},
        {0x18F4F98, "std::vector<double_t>"}, // Without length restriction

        {0x189DE88, "std::string"},
        {0x18F4FA0, "std::vector<std::string>"}, // 2 Byte length

        {0x18F3A30, "unknown_8"},
        {0x18F4FA8, "std::vector<unknown_8>"}, // Without length restriction

        {0x18F3A70, "unknown_16"},
        {0x18F4FB0, "std::vector<unknown_16>"}, // Without length restriction

        {0x18F3A50, "unknown_32"},
        {0x18F4FB8, "std::vector<unknown_32>"}, // Without length restriction

        // Type SNetPrimingOutputStep (0x18F4EF0)
        //{0x18F3A28, "SNetPrimingOutputStep"},
        //{0x18F3A90, "std::vector<SNetPrimingOutputStep>"}, // Without length restriction

        // Type SNetAssetSignature (0x18F4E88)
        //{0x18F3A20, "SNetAssetSignature"},
        //{0x18F3A78, "std::vector<SNetAssetSignature>"}, // Without length restriction

        // Might be an UUID
        {0x18F3A88, "uint128_t"},
};

[[nodiscard]] std::unordered_map<std::string, reflang::IClass*>& get_class_name_mapping() {
    auto class_map_fn = (void*(*)()) (util::exe_offset + 0x15E60F0);
    auto class_map = (std::unordered_map<std::string, reflang::IClass*>*) class_map_fn();
    return *class_map;
}

void reflang::generate_game_events() {
    std::ostringstream ostream_hpp{};
    std::ostringstream ostream_cpp{};

    const auto& class_name_mapping = get_class_name_mapping();

    std::vector<std::pair<std::string, std::string>> declared_events{};
    declared_events.reserve(class_name_mapping.size());

    ostream_hpp << "#define GAME_EVENTS_DECLARED\n";
    ostream_hpp << "\n";

    for(const auto& [ key, value ] : class_name_mapping) {
        if(key.find("NetworkEvent::") != 0) { // SNetUpgradeTowerEvent
            continue;
        }

        auto class_name{util::replace_all(key.substr(14), {{ "::", "__" }})};
        ostream_hpp << fmt::format("class {} : public GameEvent {{\n", class_name);

        std::vector<std::pair<std::string, std::string>> field_list{};
        field_list.reserve(value->get_field_count());

        bool contains_unknown_fields{false};

        /* constructor */
        {
            ostream_hpp << "    public:\n";
            ostream_hpp << util::replace_all("        explicit %class_name%(uint16_t /* event_id */, uint64_t /* sim tick */, uint8_t /* unknown u8 */);\n", {{ "%class_name%", class_name }});
            ostream_cpp << util::replace_all(R"(
%class_name%::%class_name%(uint16_t event_id, uint64_t sim_tick, uint8_t unknown_u8)
    : GameEvent(event_id, sim_tick, unknown_u8) { }
)", {{ "%class_name%", class_name }});
        }

        {
            reflang::Reference instance{};
            instance.type = value->get_type();
            instance.data_ptr = nullptr;

            ostream_hpp << "    public:\n";
            for(const auto& field : value->get_field_list()) {
                reflang::Reference field_reference{};
                value->get_field(field_reference, instance, field);

                auto rfa_type = (size_t) field_reference.type - util::exe_offset;

                auto field_type = type_mapping.find(rfa_type);
                if (field_type == type_mapping.end()) {
                    contains_unknown_fields = true;
                    ostream_hpp << fmt::format("        /* unknown {}; */;\n", field);
                    continue;
                }

                ostream_hpp << fmt::format("        {} {}{{}};\n", field_type->second, field);
                field_list.emplace_back(field, field_type->second);
            }
        }

        /* method event name */
        {
            ostream_hpp << "\n";
            ostream_hpp << "    public:\n";
            ostream_hpp << "        std::string event_name() const override;\n";

            ostream_cpp << util::replace_all(R"(
std::string %class_name%::event_name() const {
    return "%class_name%";
}
)", {{ "%class_name%", class_name }});
            ostream_cpp << "\n";
        }

        /* method describe */
        {
            ostream_hpp << "\n";
            ostream_hpp << "        void iter_fields(const iter_fields_callback_t &fn) const override;\n";

            std::ostringstream iter_fields{};

            for(const auto& [ name, type ] : field_list) {
                std::string value{};
                if(type.find("std::vector") == 0) {
                    value = util::replace_all(R"(fmt::format("{}", fmt::join(this->%var%, ", ")))", {{ "%var%", name }});
                } else {
                    value = util::replace_all("fmt::format(\"{}\", this->%var%)", {{ "%var%", name }});
                }
                iter_fields << fmt::format("    fn(\"{}\", \"{}\", {});\n", name, type, value);
            }

            ostream_cpp << util::replace_all(R"(
void %class_name%::iter_fields(const iter_fields_callback_t &fn) const {
%iter_fields%
}
)", {
                { "%class_name%", class_name },
                { "%iter_fields%", iter_fields.str() }
            });
            ostream_cpp << "\n";
        }

        /* decode method */
        if(!contains_unknown_fields) {
            std::ostringstream field_parsers{};
            for(const auto& [ name, type ] : field_list) {
                field_parsers << util::replace_all(
                    "    (void) reader.read(instance->%field_name%);\n",
                    {{ "%field_name%", name }}
                );
            }

            ostream_hpp << "        static std::unique_ptr<GameEvent> decode(const PayloadPacketBuffer& reader);\n";

            ostream_cpp << util::replace_all(R"(
std::unique_ptr<GameEvent> %class_name%::decode(const PayloadPacketBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != %event_id%) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<%class_name%>(event_id, sim_tick, unknown_u8);
%field_parsers%

    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            )", {
                    { "%event_id%", fmt::format("0x{:X}", 0x00) },
                    { "%class_name%", class_name },
                    { "%field_parsers%", field_parsers.str() },
            });
            ostream_cpp << "\n";
        } else {
            ostream_hpp << "    public:";
            ostream_hpp << R"(
        static std::unique_ptr<GameEvent> decode(const PayloadPacketBuffer& reader) {
            /* Some fields are of unknown type. We can not decode this event struct. */
            return nullptr;
        }
            )";
            ostream_hpp << "\n";
        }


        /* encode method */
        if(!contains_unknown_fields) {
            std::ostringstream field_writer{};
            for(const auto& [ name, type ] : field_list) {
                field_writer << util::replace_all(
                        "    (void) writer.write(this->%field_name%);\n",
                        {{ "%field_name%", name }}
                );
            }

            ostream_hpp << "        [[nodiscard]] bool encode(PayloadPacketBuffer& /* output */) const override;\n";

            ostream_cpp << util::replace_all(R"(
bool %class_name%::encode(PayloadPacketBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

%field_writer%

    return !writer.has_write_error();
}
            )", {
                    { "%event_id%",     fmt::format("0x{:X}", 0x00) },
                    { "%class_name%",   class_name },
                    { "%field_writer%", field_writer.str() },
            });
            ostream_cpp << "\n";
        } else {
            ostream_hpp << "        [[nodiscard]] bool encode(PayloadPacketBuffer& /* output */) const override {\n";
            ostream_hpp << "            return false;\n";
            ostream_hpp << "        }";
        }

        ostream_hpp << "};\n";

        declared_events.emplace_back(key, class_name);
    }

    ostream_hpp << "\n";
    ostream_hpp << "extern std::map<std::string, std::unique_ptr<GameEvent>(*)(const PayloadPacketBuffer&)> game_event_decoders;";

    ostream_cpp << "std::map<std::string, std::unique_ptr<GameEvent>(*)(const PayloadPacketBuffer&)> game_event_decoders{\n";
    for(const auto& [ key, klass ] : declared_events) {
        ostream_cpp << util::replace_all("    { \"%name%\", %class%::decode },\n", {{"%name%", key }, {"%class%", klass }});
    }
    ostream_cpp << "};\n";

    {
        std::ofstream file_hpp{"game_event_generated.h"};
        file_hpp << ostream_hpp.str();
    }

    {
        std::ofstream file_hpp{"game_event_generated.cpp"};
        file_hpp << ostream_cpp.str();
    }
    //logging::info("Header:\n{}", ostream_hpp.str());
    //logging::info("Source:\n{}", ostream_cpp.str());
}

// Class list 15E61D0
bool reflang::initialize(std::string&) {
//    reflang::generate_game_events();

//    auto class_map_fn = (void*(*)()) (util::exe_offset + 0x15E60F0);
//    auto class_map = (std::unordered_map<std::string, reflang::IClass*>*) class_map_fn();
//
//    auto class_type_map_fn = (std::unordered_map<reflang::type_t , reflang::IClass*>*(*)()) (util::exe_offset + 0x15E61D0);
//    auto class_type_map = class_type_map_fn();
//
//    // Types will be registered within the main function (reflang_register_en_de_coders)
//    auto ende_coder_map = (std::unordered_map<reflang::type_t, std::pair<encoder_fn_t, decoder_fn_t>>*) (util::exe_offset + 0x1CEE890);
//
//    for(const auto& [ key, value ] : *class_map) {
//        if(key.find("NetworkEvent::SNetUpgradeTowerEvent") != 0) {
//            continue;
//        }
//
//        logging::info("{} (0x{:X}) Type: 0x{:X}", key, *(uintptr_t*) value - util::exe_offset, (uintptr_t) value->get_type() - util::exe_offset);
//        auto mit = class_type_map->find(value->get_type());
//        if(mit == class_type_map->end()) {
//            logging::warn(" Missing type -> class mapping");
//        } else if(mit->second == value) {
//            logging::info(" type -> class mapping ok");
//        } else {
//            logging::warn(" type -> class mapping different");
//        }
//
//        for(const auto& field : value->get_field_list()) {
//            reflang::Reference instance{};
//            instance.type = value->get_type();
//            instance.data_ptr = nullptr;
//
//            reflang::Reference field_reference{};
//            value->get_field(field_reference, instance, field);
//
//            auto rfa_type = (size_t) field_reference.type - util::exe_offset;
//            std::string type_name{};
//            auto type_it = type_mapping.find(rfa_type);
//            if(type_it != type_mapping.end()) {
//                type_name = type_it->second;
//            } else {
//                type_name = "unknown";
//            }
//
//            logging::info(" {} (Offset: 0x{:X}, Type: 0x{:X} | {})", field, (size_t) field_reference.data_ptr, rfa_type, type_name);
//            // type_mapping
// //            if(field_reference.type) {
// //                auto it = ende_coder_map->find(field_reference.type);
// //                if(it == ende_coder_map->end()) {
// //                    logging::info("  En/decoder unknown.");
// //                } else {
// //                    logging::info("  Encoder: 0x{:X}", (size_t) it->second.first - util::exe_offset);
// //                    logging::info("  Decoder: 0x{:X}", (size_t) it->second.second - util::exe_offset);
// //                }
// //            }
//        }
//    }

    return true;
}

void reflang::dump_event_map() {
    for(const auto& [ event_id, vtable_ptr ] : game_event_mapping_vtable1) {
        auto vtable = (void*) (vtable_ptr + util::exe_offset);
        auto instance = (reflang::IClass*) &vtable;

        logging::info("{{ 0x{:X}, \"{}\" }},", event_id, instance->get_name());
    }
}