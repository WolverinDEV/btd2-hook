#define GAME_EVENTS_DECLARED

class SNetLobbyLoadedEvent : public GameEvent {
    public:
        explicit SNetLobbyLoadedEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint64_t request_map_asset_id{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetDebug_SimulationBundleUploadEvent : public GameEvent {
    public:
        explicit SNetDebug_SimulationBundleUploadEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        /* unknown asset_signature; */;
        std::vector<uint8_t> simulation_bundle_data{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
    public:
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader) {
            /* Some fields are of unknown type. We can not decode this event struct. */
            return nullptr;
        }
            
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override {
            return false;
        }};
class SNetRequestMapSkipEvent : public GameEvent {
    public:
        explicit SNetRequestMapSkipEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint64_t explicitly_set_map_id{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetDebug_LoadTestBytesEvent : public GameEvent {
    public:
        explicit SNetDebug_LoadTestBytesEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        std::vector<uint8_t> bytes{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetShowEmoteEvent : public GameEvent {
    public:
        explicit SNetShowEmoteEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint8_t side{};
        uint8_t is_visual_emote{};
        uint8_t index{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetRequestFirePlayerAbilityEvent : public GameEvent {
    public:
        explicit SNetRequestFirePlayerAbilityEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint8_t target_side{};
        uint64_t sim_tick{};
        uint32_t ability_type{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetSendFirstBloodEvent : public GameEvent {
    public:
        explicit SNetSendFirstBloodEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint8_t side{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetQueueBloonSetEvent : public GameEvent {
    public:
        explicit SNetQueueBloonSetEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint64_t sim_tick{};
        uint32_t bloon_set_id{};
        uint16_t bloon_features{};
        uint32_t path_index{};
        uint8_t is_self_sent{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetSendCriticalLifeLostEvent : public GameEvent {
    public:
        explicit SNetSendCriticalLifeLostEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint64_t tick{};
        float_t life_before{};
        float_t life_after{};
        uint8_t reason{};
        uint8_t bloon_source{};
        uint32_t bloon_type{};
        uint16_t bloon_features{};
        float_t bloon_pos_x{};
        float_t bloon_pos_y{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetRequestWithdrawCashFromTowerEvent : public GameEvent {
    public:
        explicit SNetRequestWithdrawCashFromTowerEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint64_t sim_tick{};
        uint64_t net_id{};
        uint32_t net_id_version{};
        uint8_t withdraw_from_all{};
        uint64_t eco_receipt_id{};
        uint32_t tower_type{};
        uint32_t upgrade_combo{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetPlaceTowerEvent : public GameEvent {
    public:
        explicit SNetPlaceTowerEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint64_t sim_tick{};
        uint32_t tower_type{};
        uint64_t opaque_id{};
        float_t pos_x{};
        float_t pos_y{};
        uint32_t layer{};
        float_t cost{};
        uint64_t tower_entity_id{};
        uint32_t tower_entity_id_version{};
        uint64_t parent_entity_id{};
        uint32_t parent_entity_id_version{};
        uint32_t upgrade_combo{};
        uint32_t preset_modifier_id_checksum{};
        uint32_t target_mode_override{};
        uint8_t reverse_flight_path{};
        uint64_t eco_receipt_id{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetGameResultEvent : public GameEvent {
    public:
        explicit SNetGameResultEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint32_t result{};
        uint8_t side{};
        uint32_t reason{};
        uint8_t final_blow_bloon_source{};
        uint32_t final_blow_bloon_type{};
        uint16_t final_blow_bloon_features{};
        float_t final_blow_bloon_pos_x{};
        float_t final_blow_bloon_pos_y{};
        uint128_t leaderboard_deltas{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetGameLoadedEvent : public GameEvent {
    public:
        explicit SNetGameLoadedEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetTriggerMapSkipEvent : public GameEvent {
    public:
        explicit SNetTriggerMapSkipEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint64_t new_map_id{};
        uint32_t explicitly_set{};
        float_t lobby_ready_countdown{};
        uint32_t requesting_side_mask{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetPlayerShieldActivatedEvent : public GameEvent {
    public:
        explicit SNetPlayerShieldActivatedEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint64_t sim_tick{};
        uint64_t activate_tick{};
        uint64_t deactivate_tick{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetBlingLoadoutEvent : public GameEvent {
    public:
        explicit SNetBlingLoadoutEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        std::string visual_emote_0{};
        std::string visual_emote_1{};
        std::string visual_emote_2{};
        std::string visual_emote_3{};
        std::string visual_emote_4{};
        std::string visual_emote_5{};
        std::string visual_emote_6{};
        std::string visual_emote_7{};
        std::string visual_emote_8{};
        std::string visual_emote_9{};
        std::string speech_emote_0{};
        std::string speech_emote_1{};
        std::string speech_emote_2{};
        std::string speech_emote_3{};
        std::string bloon_decal{};
        std::string bloon_skin_moab{};
        std::string bloon_skin_bfb{};
        std::string bloon_skin_zomg{};
        std::string bloon_skin_ddt{};
        std::string bloon_skin_bad{};
        std::string common_vfx_pop{};
        std::string common_vfx_boost_tower{};
        std::string common_vfx_boost_bloon{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetRequestSendTauntEvent : public GameEvent {
    public:
        explicit SNetRequestSendTauntEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint32_t taunt_index{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetTickEvent : public GameEvent {
    public:
        explicit SNetTickEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        unknown_16 flags{};
        uint32_t net_tick{};
        uint64_t sim_tick{};
        uint32_t latency{};
        uint32_t jitter{};
        uint64_t bloons_created{};
        uint64_t bloons_leaked{};
        uint64_t bloons_popped{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetScheduleRoundStartEvent : public GameEvent {
    public:
        explicit SNetScheduleRoundStartEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint64_t round_starts_at_tick{};
        uint32_t round_idx{};
        uint8_t round_skip_prevention_enabled{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetSetBadgesEvent : public GameEvent {
    public:
        explicit SNetSetBadgesEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        /* unknown badge1; */;
        /* unknown badge2; */;
        /* unknown badge3; */;
        uint8_t badge_count{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
    public:
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader) {
            /* Some fields are of unknown type. We can not decode this event struct. */
            return nullptr;
        }
            
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override {
            return false;
        }};
class SNetInitPlayerResponseEvent : public GameEvent {
    public:
        explicit SNetInitPlayerResponseEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        /* unknown asset_signature; */;

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
    public:
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader) {
            /* Some fields are of unknown type. We can not decode this event struct. */
            return nullptr;
        }
            
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override {
            return false;
        }};
class SNetSetBadgesEvent__Badge : public GameEvent {
    public:
        explicit SNetSetBadgesEvent__Badge(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint8_t season_index{};
        uint8_t type{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetPatrolPointsSelectedEvent : public GameEvent {
    public:
        explicit SNetPatrolPointsSelectedEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint64_t net_id{};
        uint32_t net_id_version{};
        float_t p0_x{};
        float_t p0_y{};
        float_t p1_x{};
        float_t p1_y{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetChangeTargetModeTowerEvent : public GameEvent {
    public:
        explicit SNetChangeTargetModeTowerEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint64_t sim_tick{};
        uint32_t target_mode{};
        uint64_t net_id{};
        uint32_t net_id_version{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetRequestSwitchFlyingDirectionEvent : public GameEvent {
    public:
        explicit SNetRequestSwitchFlyingDirectionEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint64_t sim_tick{};
        uint64_t net_id{};
        uint32_t net_id_version{};
        uint8_t is_flying_clockwise{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetStartGameEvent : public GameEvent {
    public:
        explicit SNetStartGameEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint64_t first_round_tick{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetTowerLoadoutEvent : public GameEvent {
    public:
        explicit SNetTowerLoadoutEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint32_t hero_slot{};
        uint32_t tower_slot_0{};
        uint32_t tower_slot_1{};
        uint32_t tower_slot_2{};
        std::string hero_taunt_slot_0{};
        std::string hero_taunt_slot_1{};
        std::string hero_taunt_slot_2{};
        std::string hero_tower_animations{};
        std::string tower_animations_0{};
        std::string tower_animations_1{};
        std::string tower_animations_2{};
        std::string hero_vfx_loadout{};
        std::string tower_vfx_loadout_0{};
        std::string tower_vfx_loadout_1{};
        std::string tower_vfx_loadout_2{};
        std::string custom_name_0{};
        std::string custom_name_1{};
        std::string custom_name_2{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetSendSurrenderNetEvent : public GameEvent {
    public:
        explicit SNetSendSurrenderNetEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint64_t tick{};
        uint8_t side{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetInitPlayerPostponedEvent : public GameEvent {
    public:
        explicit SNetInitPlayerPostponedEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint8_t due_to_missing_assets_on_referee{};
        uint8_t flags{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetLastRoundGameEndRequestEvent : public GameEvent {
    public:
        explicit SNetLastRoundGameEndRequestEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint8_t side{};
        uint8_t please_end_the_game{};
        uint64_t sim_tick{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetRequestFireWeaponEvent : public GameEvent {
    public:
        explicit SNetRequestFireWeaponEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint64_t sim_tick{};
        uint64_t net_id{};
        uint32_t net_id_version{};
        /* unknown priming_outputs; */;
        uint64_t eco_receipt_id{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
    public:
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader) {
            /* Some fields are of unknown type. We can not decode this event struct. */
            return nullptr;
        }
            
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override {
            return false;
        }};
class SNetMoveToTowerSelectEvent : public GameEvent {
    public:
        explicit SNetMoveToTowerSelectEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        float_t lobby_ready_countdown{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetDidFirePlayerAbilityEvent : public GameEvent {
    public:
        explicit SNetDidFirePlayerAbilityEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint64_t sim_tick{};
        uint32_t ability_type{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetTargetTouchPointEvent : public GameEvent {
    public:
        explicit SNetTargetTouchPointEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint64_t sim_tick{};
        float_t point_x{};
        float_t point_y{};
        uint8_t released{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetAssetBlobUploadEvent : public GameEvent {
    public:
        explicit SNetAssetBlobUploadEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        /* unknown asset_signature; */;
        uint64_t asset_type{};
        std::vector<uint8_t> data{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
    public:
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader) {
            /* Some fields are of unknown type. We can not decode this event struct. */
            return nullptr;
        }
            
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override {
            return false;
        }};
class SNetBombBlitzActivatedEvent : public GameEvent {
    public:
        explicit SNetBombBlitzActivatedEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint64_t sim_tick{};
        uint64_t activate_tick{};
        uint64_t deactivate_tick{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetCenterPointSelectedEvent : public GameEvent {
    public:
        explicit SNetCenterPointSelectedEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint64_t net_id{};
        uint32_t net_id_version{};
        float_t p0_x{};
        float_t p0_y{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetCollectablesCollectedEvent : public GameEvent {
    public:
        explicit SNetCollectablesCollectedEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        /* unknown entities; */;
        /* unknown entity_parents; */;

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
    public:
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader) {
            /* Some fields are of unknown type. We can not decode this event struct. */
            return nullptr;
        }
            
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override {
            return false;
        }};
class SNetConfirmRoundStartEvent : public GameEvent {
    public:
        explicit SNetConfirmRoundStartEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint64_t round_starts_at_tick{};
        uint32_t round_idx{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetDebug_LoadScenarioEvent : public GameEvent {
    public:
        explicit SNetDebug_LoadScenarioEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint32_t scenario_number{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetDebug_SetSimSpeedEvent : public GameEvent {
    public:
        explicit SNetDebug_SetSimSpeedEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        float_t speed{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetDebug_StartRoundEvent : public GameEvent {
    public:
        explicit SNetDebug_StartRoundEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint64_t round_starts_at_tick{};
        uint32_t round_idx{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetInitPlayerRequestEvent : public GameEvent {
    public:
        explicit SNetInitPlayerRequestEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint32_t rng_seed{};
        std::vector<uint8_t> player_ids_by_side{};
        uint16_t lobby_generation{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetDidQueueBloonSetEvent : public GameEvent {
    public:
        explicit SNetDidQueueBloonSetEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint64_t sim_tick{};
        uint32_t bloon_set_id{};
        uint16_t bloon_features{};
        uint32_t path_index{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetEcoUpdatedEvent : public GameEvent {
    public:
        explicit SNetEcoUpdatedEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        float_t cash{};
        float_t income{};
        uint64_t eco_receipt_id{};
        unknown_8 context{};
        uint64_t subcontext{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetFirePlayerAbilityEvent : public GameEvent {
    public:
        explicit SNetFirePlayerAbilityEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint64_t sim_tick{};
        uint32_t ability_type{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetInitPlayerCompleteEvent : public GameEvent {
    public:
        explicit SNetInitPlayerCompleteEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetLobbyErrorEvent : public GameEvent {
    public:
        explicit SNetLobbyErrorEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        unknown_32 error_code{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetLockInPlacePointSelectedEvent : public GameEvent {
    public:
        explicit SNetLockInPlacePointSelectedEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint64_t net_id{};
        uint32_t net_id_version{};
        float_t p0_x{};
        float_t p0_y{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetReadyToPlayEvent : public GameEvent {
    public:
        explicit SNetReadyToPlayEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetReadyUpEvent : public GameEvent {
    public:
        explicit SNetReadyUpEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint16_t ready_phase{};
        uint16_t lobby_generation{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetRefereeSelectedTowerLoadoutEvent : public GameEvent {
    public:
        explicit SNetRefereeSelectedTowerLoadoutEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint32_t hero_slot{};
        uint32_t tower_slot_0{};
        uint32_t tower_slot_1{};
        uint32_t tower_slot_2{};
        uint8_t flags{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetRemoveBlockerEvent : public GameEvent {
    public:
        explicit SNetRemoveBlockerEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint64_t sim_tick{};
        uint64_t blocker_group{};
        uint64_t eco_receipt_id{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetRemoveTowerEvent : public GameEvent {
    public:
        explicit SNetRemoveTowerEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint64_t sim_tick{};
        uint64_t net_id{};
        uint32_t net_id_version{};
        uint32_t tower_type{};
        uint32_t tower_upgrade_level{};
        uint64_t eco_receipt_id{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetRequestCurrentHealthEvent : public GameEvent {
    public:
        explicit SNetRequestCurrentHealthEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint64_t tick{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetRequestQueueBloonSetEvent : public GameEvent {
    public:
        explicit SNetRequestQueueBloonSetEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint8_t target_side{};
        uint64_t sim_tick{};
        uint32_t bloon_set_id{};
        uint16_t bloon_features{};
        uint32_t path_index{};
        float_t cost{};
        float_t income_change{};
        uint64_t eco_receipt_id{};
        uint8_t self_send_flag{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetRequestSetTowerTargetPointEvent : public GameEvent {
    public:
        explicit SNetRequestSetTowerTargetPointEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint64_t sim_tick{};
        uint64_t net_id{};
        uint32_t net_id_version{};
        float_t pos_x{};
        float_t pos_y{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetRequestSimSnapshotEvent : public GameEvent {
    public:
        explicit SNetRequestSimSnapshotEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint64_t take_snapshot_at_tick{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetRequestStartTowerFlightPathRejoinEvent : public GameEvent {
    public:
        explicit SNetRequestStartTowerFlightPathRejoinEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint64_t sim_tick{};
        uint64_t net_id{};
        uint32_t net_id_version{};
        uint64_t start_path_tick{};
        uint64_t end_path_tick{};
        float_t p0_x{};
        float_t p0_y{};
        float_t p1_x{};
        float_t p1_y{};
        float_t p2_x{};
        float_t p2_y{};
        float_t p3_x{};
        float_t p3_y{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetRequestSwitchHandEvent : public GameEvent {
    public:
        explicit SNetRequestSwitchHandEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint64_t sim_tick{};
        uint64_t net_id{};
        uint32_t net_id_version{};
        uint8_t is_left_handed{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetSendLifeChangedEvent : public GameEvent {
    public:
        explicit SNetSendLifeChangedEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint64_t tick{};
        float_t life_before{};
        float_t life_after{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetSetGameMapEvent : public GameEvent {
    public:
        explicit SNetSetGameMapEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint64_t map_asset_id{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetSetNameEvent : public GameEvent {
    public:
        explicit SNetSetNameEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint8_t is_vip{};
        std::string avatar_id{};
        std::string banner_id{};
        std::string border_id{};
        std::string nkapi_id{};
        uint8_t can_do_master_match{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetSimSnapshotEvent : public GameEvent {
    public:
        explicit SNetSimSnapshotEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint64_t tick{};
        std::vector<uint8_t> data{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetStartLoadingEvent : public GameEvent {
    public:
        explicit SNetStartLoadingEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint64_t map{};
        uint32_t left_hero_slot{};
        uint32_t left_tower_slot_0{};
        uint32_t left_tower_slot_1{};
        uint32_t left_tower_slot_2{};
        uint32_t right_hero_slot{};
        uint32_t right_tower_slot_0{};
        uint32_t right_tower_slot_1{};
        uint32_t right_tower_slot_2{};
        uint8_t result{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetStartLobbyEvent : public GameEvent {
    public:
        explicit SNetStartLobbyEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint8_t lobby_clock{};
        uint64_t map_asset_id{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};
class SNetUpgradeTowerEvent : public GameEvent {
    public:
        explicit SNetUpgradeTowerEvent(uint16_t /* event_id */, uint64_t /* unknown 64 */, uint8_t /* unknown u8 */);
    public:
        uint64_t sim_tick{};
        uint8_t path{};
        int8_t to_level{};
        float_t cost{};
        float_t cost_xp{};
        uint64_t net_id{};
        uint32_t net_id_version{};
        uint32_t tower_type{};
        uint64_t eco_receipt_id{};

    public:
        std::string event_name() const override;

        void iter_fields(const iter_fields_callback_t &fn) const override;
        static std::unique_ptr<GameEvent> decode(const NetworkMessageBuffer& reader);
        [[nodiscard]] bool encode(NetworkMessageBuffer& /* output */) const override;
};

extern std::map<std::string, std::unique_ptr<GameEvent>(*)(const NetworkMessageBuffer&)> game_event_decoders;