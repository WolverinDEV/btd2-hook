
SNetLobbyLoadedEvent::SNetLobbyLoadedEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetLobbyLoadedEvent::event_name() const {
    return "SNetLobbyLoadedEvent";
}


void SNetLobbyLoadedEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("request_map_asset_id", "uint64_t", fmt::format("{}", this->request_map_asset_id));

}


std::unique_ptr<GameEvent> SNetLobbyLoadedEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetLobbyLoadedEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->request_map_asset_id);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetLobbyLoadedEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->request_map_asset_id);


    return !writer.has_write_error();
}
            

SNetDebug_SimulationBundleUploadEvent::SNetDebug_SimulationBundleUploadEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetDebug_SimulationBundleUploadEvent::event_name() const {
    return "SNetDebug_SimulationBundleUploadEvent";
}


void SNetDebug_SimulationBundleUploadEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("simulation_bundle_data", "std::vector<uint8_t>", fmt::format("{}", fmt::join(this->simulation_bundle_data, ", ")));

}


SNetRequestMapSkipEvent::SNetRequestMapSkipEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetRequestMapSkipEvent::event_name() const {
    return "SNetRequestMapSkipEvent";
}


void SNetRequestMapSkipEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("explicitly_set_map_id", "uint64_t", fmt::format("{}", this->explicitly_set_map_id));

}


std::unique_ptr<GameEvent> SNetRequestMapSkipEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetRequestMapSkipEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->explicitly_set_map_id);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetRequestMapSkipEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->explicitly_set_map_id);


    return !writer.has_write_error();
}
            

SNetDebug_LoadTestBytesEvent::SNetDebug_LoadTestBytesEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetDebug_LoadTestBytesEvent::event_name() const {
    return "SNetDebug_LoadTestBytesEvent";
}


void SNetDebug_LoadTestBytesEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("bytes", "std::vector<uint8_t>", fmt::format("{}", fmt::join(this->bytes, ", ")));

}


std::unique_ptr<GameEvent> SNetDebug_LoadTestBytesEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetDebug_LoadTestBytesEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->bytes);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetDebug_LoadTestBytesEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->bytes);


    return !writer.has_write_error();
}
            

SNetShowEmoteEvent::SNetShowEmoteEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetShowEmoteEvent::event_name() const {
    return "SNetShowEmoteEvent";
}


void SNetShowEmoteEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("side", "uint8_t", fmt::format("{}", this->side));
    fn("is_visual_emote", "uint8_t", fmt::format("{}", this->is_visual_emote));
    fn("index", "uint8_t", fmt::format("{}", this->index));

}


std::unique_ptr<GameEvent> SNetShowEmoteEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetShowEmoteEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->side);
    (void) reader.read(instance->is_visual_emote);
    (void) reader.read(instance->index);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetShowEmoteEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->side);
    (void) writer.write(this->is_visual_emote);
    (void) writer.write(this->index);


    return !writer.has_write_error();
}
            

SNetRequestFirePlayerAbilityEvent::SNetRequestFirePlayerAbilityEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetRequestFirePlayerAbilityEvent::event_name() const {
    return "SNetRequestFirePlayerAbilityEvent";
}


void SNetRequestFirePlayerAbilityEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("target_side", "uint8_t", fmt::format("{}", this->target_side));
    fn("sim_tick", "uint64_t", fmt::format("{}", this->sim_tick));
    fn("ability_type", "uint32_t", fmt::format("{}", this->ability_type));

}


std::unique_ptr<GameEvent> SNetRequestFirePlayerAbilityEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetRequestFirePlayerAbilityEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->target_side);
    (void) reader.read(instance->sim_tick);
    (void) reader.read(instance->ability_type);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetRequestFirePlayerAbilityEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->target_side);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->ability_type);


    return !writer.has_write_error();
}
            

SNetSendFirstBloodEvent::SNetSendFirstBloodEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetSendFirstBloodEvent::event_name() const {
    return "SNetSendFirstBloodEvent";
}


void SNetSendFirstBloodEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("side", "uint8_t", fmt::format("{}", this->side));

}


std::unique_ptr<GameEvent> SNetSendFirstBloodEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetSendFirstBloodEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->side);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetSendFirstBloodEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->side);


    return !writer.has_write_error();
}
            

SNetQueueBloonSetEvent::SNetQueueBloonSetEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetQueueBloonSetEvent::event_name() const {
    return "SNetQueueBloonSetEvent";
}


void SNetQueueBloonSetEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("sim_tick", "uint64_t", fmt::format("{}", this->sim_tick));
    fn("bloon_set_id", "uint32_t", fmt::format("{}", this->bloon_set_id));
    fn("bloon_features", "uint16_t", fmt::format("{}", this->bloon_features));
    fn("path_index", "uint32_t", fmt::format("{}", this->path_index));
    fn("is_self_sent", "uint8_t", fmt::format("{}", this->is_self_sent));

}


std::unique_ptr<GameEvent> SNetQueueBloonSetEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetQueueBloonSetEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->sim_tick);
    (void) reader.read(instance->bloon_set_id);
    (void) reader.read(instance->bloon_features);
    (void) reader.read(instance->path_index);
    (void) reader.read(instance->is_self_sent);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetQueueBloonSetEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->sim_tick);
    (void) writer.write(this->bloon_set_id);
    (void) writer.write(this->bloon_features);
    (void) writer.write(this->path_index);
    (void) writer.write(this->is_self_sent);


    return !writer.has_write_error();
}
            

SNetSendCriticalLifeLostEvent::SNetSendCriticalLifeLostEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetSendCriticalLifeLostEvent::event_name() const {
    return "SNetSendCriticalLifeLostEvent";
}


void SNetSendCriticalLifeLostEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("tick", "uint64_t", fmt::format("{}", this->tick));
    fn("life_before", "float_t", fmt::format("{}", this->life_before));
    fn("life_after", "float_t", fmt::format("{}", this->life_after));
    fn("reason", "uint8_t", fmt::format("{}", this->reason));
    fn("bloon_source", "uint8_t", fmt::format("{}", this->bloon_source));
    fn("bloon_type", "uint32_t", fmt::format("{}", this->bloon_type));
    fn("bloon_features", "uint16_t", fmt::format("{}", this->bloon_features));
    fn("bloon_pos_x", "float_t", fmt::format("{}", this->bloon_pos_x));
    fn("bloon_pos_y", "float_t", fmt::format("{}", this->bloon_pos_y));

}


std::unique_ptr<GameEvent> SNetSendCriticalLifeLostEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetSendCriticalLifeLostEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->tick);
    (void) reader.read(instance->life_before);
    (void) reader.read(instance->life_after);
    (void) reader.read(instance->reason);
    (void) reader.read(instance->bloon_source);
    (void) reader.read(instance->bloon_type);
    (void) reader.read(instance->bloon_features);
    (void) reader.read(instance->bloon_pos_x);
    (void) reader.read(instance->bloon_pos_y);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetSendCriticalLifeLostEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->tick);
    (void) writer.write(this->life_before);
    (void) writer.write(this->life_after);
    (void) writer.write(this->reason);
    (void) writer.write(this->bloon_source);
    (void) writer.write(this->bloon_type);
    (void) writer.write(this->bloon_features);
    (void) writer.write(this->bloon_pos_x);
    (void) writer.write(this->bloon_pos_y);


    return !writer.has_write_error();
}
            

SNetRequestWithdrawCashFromTowerEvent::SNetRequestWithdrawCashFromTowerEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetRequestWithdrawCashFromTowerEvent::event_name() const {
    return "SNetRequestWithdrawCashFromTowerEvent";
}


void SNetRequestWithdrawCashFromTowerEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("sim_tick", "uint64_t", fmt::format("{}", this->sim_tick));
    fn("net_id", "uint64_t", fmt::format("{}", this->net_id));
    fn("net_id_version", "uint32_t", fmt::format("{}", this->net_id_version));
    fn("withdraw_from_all", "uint8_t", fmt::format("{}", this->withdraw_from_all));
    fn("eco_receipt_id", "uint64_t", fmt::format("{}", this->eco_receipt_id));
    fn("tower_type", "uint32_t", fmt::format("{}", this->tower_type));
    fn("upgrade_combo", "uint32_t", fmt::format("{}", this->upgrade_combo));

}


std::unique_ptr<GameEvent> SNetRequestWithdrawCashFromTowerEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetRequestWithdrawCashFromTowerEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->sim_tick);
    (void) reader.read(instance->net_id);
    (void) reader.read(instance->net_id_version);
    (void) reader.read(instance->withdraw_from_all);
    (void) reader.read(instance->eco_receipt_id);
    (void) reader.read(instance->tower_type);
    (void) reader.read(instance->upgrade_combo);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetRequestWithdrawCashFromTowerEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->sim_tick);
    (void) writer.write(this->net_id);
    (void) writer.write(this->net_id_version);
    (void) writer.write(this->withdraw_from_all);
    (void) writer.write(this->eco_receipt_id);
    (void) writer.write(this->tower_type);
    (void) writer.write(this->upgrade_combo);


    return !writer.has_write_error();
}
            

SNetPlaceTowerEvent::SNetPlaceTowerEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetPlaceTowerEvent::event_name() const {
    return "SNetPlaceTowerEvent";
}


void SNetPlaceTowerEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("sim_tick", "uint64_t", fmt::format("{}", this->sim_tick));
    fn("tower_type", "uint32_t", fmt::format("{}", this->tower_type));
    fn("opaque_id", "uint64_t", fmt::format("{}", this->opaque_id));
    fn("pos_x", "float_t", fmt::format("{}", this->pos_x));
    fn("pos_y", "float_t", fmt::format("{}", this->pos_y));
    fn("layer", "uint32_t", fmt::format("{}", this->layer));
    fn("cost", "float_t", fmt::format("{}", this->cost));
    fn("tower_entity_id", "uint64_t", fmt::format("{}", this->tower_entity_id));
    fn("tower_entity_id_version", "uint32_t", fmt::format("{}", this->tower_entity_id_version));
    fn("parent_entity_id", "uint64_t", fmt::format("{}", this->parent_entity_id));
    fn("parent_entity_id_version", "uint32_t", fmt::format("{}", this->parent_entity_id_version));
    fn("upgrade_combo", "uint32_t", fmt::format("{}", this->upgrade_combo));
    fn("preset_modifier_id_checksum", "uint32_t", fmt::format("{}", this->preset_modifier_id_checksum));
    fn("target_mode_override", "uint32_t", fmt::format("{}", this->target_mode_override));
    fn("reverse_flight_path", "uint8_t", fmt::format("{}", this->reverse_flight_path));
    fn("eco_receipt_id", "uint64_t", fmt::format("{}", this->eco_receipt_id));

}


std::unique_ptr<GameEvent> SNetPlaceTowerEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetPlaceTowerEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->sim_tick);
    (void) reader.read(instance->tower_type);
    (void) reader.read(instance->opaque_id);
    (void) reader.read(instance->pos_x);
    (void) reader.read(instance->pos_y);
    (void) reader.read(instance->layer);
    (void) reader.read(instance->cost);
    (void) reader.read(instance->tower_entity_id);
    (void) reader.read(instance->tower_entity_id_version);
    (void) reader.read(instance->parent_entity_id);
    (void) reader.read(instance->parent_entity_id_version);
    (void) reader.read(instance->upgrade_combo);
    (void) reader.read(instance->preset_modifier_id_checksum);
    (void) reader.read(instance->target_mode_override);
    (void) reader.read(instance->reverse_flight_path);
    (void) reader.read(instance->eco_receipt_id);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetPlaceTowerEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->sim_tick);
    (void) writer.write(this->tower_type);
    (void) writer.write(this->opaque_id);
    (void) writer.write(this->pos_x);
    (void) writer.write(this->pos_y);
    (void) writer.write(this->layer);
    (void) writer.write(this->cost);
    (void) writer.write(this->tower_entity_id);
    (void) writer.write(this->tower_entity_id_version);
    (void) writer.write(this->parent_entity_id);
    (void) writer.write(this->parent_entity_id_version);
    (void) writer.write(this->upgrade_combo);
    (void) writer.write(this->preset_modifier_id_checksum);
    (void) writer.write(this->target_mode_override);
    (void) writer.write(this->reverse_flight_path);
    (void) writer.write(this->eco_receipt_id);


    return !writer.has_write_error();
}
            

SNetGameResultEvent::SNetGameResultEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetGameResultEvent::event_name() const {
    return "SNetGameResultEvent";
}


void SNetGameResultEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("result", "uint32_t", fmt::format("{}", this->result));
    fn("side", "uint8_t", fmt::format("{}", this->side));
    fn("reason", "uint32_t", fmt::format("{}", this->reason));
    fn("final_blow_bloon_source", "uint8_t", fmt::format("{}", this->final_blow_bloon_source));
    fn("final_blow_bloon_type", "uint32_t", fmt::format("{}", this->final_blow_bloon_type));
    fn("final_blow_bloon_features", "uint16_t", fmt::format("{}", this->final_blow_bloon_features));
    fn("final_blow_bloon_pos_x", "float_t", fmt::format("{}", this->final_blow_bloon_pos_x));
    fn("final_blow_bloon_pos_y", "float_t", fmt::format("{}", this->final_blow_bloon_pos_y));
    fn("leaderboard_deltas", "uint128_t", fmt::format("{}", this->leaderboard_deltas));

}


std::unique_ptr<GameEvent> SNetGameResultEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetGameResultEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->result);
    (void) reader.read(instance->side);
    (void) reader.read(instance->reason);
    (void) reader.read(instance->final_blow_bloon_source);
    (void) reader.read(instance->final_blow_bloon_type);
    (void) reader.read(instance->final_blow_bloon_features);
    (void) reader.read(instance->final_blow_bloon_pos_x);
    (void) reader.read(instance->final_blow_bloon_pos_y);
    (void) reader.read(instance->leaderboard_deltas);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetGameResultEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->result);
    (void) writer.write(this->side);
    (void) writer.write(this->reason);
    (void) writer.write(this->final_blow_bloon_source);
    (void) writer.write(this->final_blow_bloon_type);
    (void) writer.write(this->final_blow_bloon_features);
    (void) writer.write(this->final_blow_bloon_pos_x);
    (void) writer.write(this->final_blow_bloon_pos_y);
    (void) writer.write(this->leaderboard_deltas);


    return !writer.has_write_error();
}
            

SNetGameLoadedEvent::SNetGameLoadedEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetGameLoadedEvent::event_name() const {
    return "SNetGameLoadedEvent";
}


void SNetGameLoadedEvent::iter_fields(const iter_fields_callback_t &fn) const {

}


std::unique_ptr<GameEvent> SNetGameLoadedEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetGameLoadedEvent>(event_id, sim_tick, unknown_u8);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetGameLoadedEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);



    return !writer.has_write_error();
}
            

SNetTriggerMapSkipEvent::SNetTriggerMapSkipEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetTriggerMapSkipEvent::event_name() const {
    return "SNetTriggerMapSkipEvent";
}


void SNetTriggerMapSkipEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("new_map_id", "uint64_t", fmt::format("{}", this->new_map_id));
    fn("explicitly_set", "uint32_t", fmt::format("{}", this->explicitly_set));
    fn("lobby_ready_countdown", "float_t", fmt::format("{}", this->lobby_ready_countdown));
    fn("requesting_side_mask", "uint32_t", fmt::format("{}", this->requesting_side_mask));

}


std::unique_ptr<GameEvent> SNetTriggerMapSkipEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetTriggerMapSkipEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->new_map_id);
    (void) reader.read(instance->explicitly_set);
    (void) reader.read(instance->lobby_ready_countdown);
    (void) reader.read(instance->requesting_side_mask);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetTriggerMapSkipEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->new_map_id);
    (void) writer.write(this->explicitly_set);
    (void) writer.write(this->lobby_ready_countdown);
    (void) writer.write(this->requesting_side_mask);


    return !writer.has_write_error();
}
            

SNetPlayerShieldActivatedEvent::SNetPlayerShieldActivatedEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetPlayerShieldActivatedEvent::event_name() const {
    return "SNetPlayerShieldActivatedEvent";
}


void SNetPlayerShieldActivatedEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("sim_tick", "uint64_t", fmt::format("{}", this->sim_tick));
    fn("activate_tick", "uint64_t", fmt::format("{}", this->activate_tick));
    fn("deactivate_tick", "uint64_t", fmt::format("{}", this->deactivate_tick));

}


std::unique_ptr<GameEvent> SNetPlayerShieldActivatedEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetPlayerShieldActivatedEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->sim_tick);
    (void) reader.read(instance->activate_tick);
    (void) reader.read(instance->deactivate_tick);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetPlayerShieldActivatedEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->sim_tick);
    (void) writer.write(this->activate_tick);
    (void) writer.write(this->deactivate_tick);


    return !writer.has_write_error();
}
            

SNetBlingLoadoutEvent::SNetBlingLoadoutEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetBlingLoadoutEvent::event_name() const {
    return "SNetBlingLoadoutEvent";
}


void SNetBlingLoadoutEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("visual_emote_0", "std::string", fmt::format("{}", this->visual_emote_0));
    fn("visual_emote_1", "std::string", fmt::format("{}", this->visual_emote_1));
    fn("visual_emote_2", "std::string", fmt::format("{}", this->visual_emote_2));
    fn("visual_emote_3", "std::string", fmt::format("{}", this->visual_emote_3));
    fn("visual_emote_4", "std::string", fmt::format("{}", this->visual_emote_4));
    fn("visual_emote_5", "std::string", fmt::format("{}", this->visual_emote_5));
    fn("visual_emote_6", "std::string", fmt::format("{}", this->visual_emote_6));
    fn("visual_emote_7", "std::string", fmt::format("{}", this->visual_emote_7));
    fn("visual_emote_8", "std::string", fmt::format("{}", this->visual_emote_8));
    fn("visual_emote_9", "std::string", fmt::format("{}", this->visual_emote_9));
    fn("speech_emote_0", "std::string", fmt::format("{}", this->speech_emote_0));
    fn("speech_emote_1", "std::string", fmt::format("{}", this->speech_emote_1));
    fn("speech_emote_2", "std::string", fmt::format("{}", this->speech_emote_2));
    fn("speech_emote_3", "std::string", fmt::format("{}", this->speech_emote_3));
    fn("bloon_decal", "std::string", fmt::format("{}", this->bloon_decal));
    fn("bloon_skin_moab", "std::string", fmt::format("{}", this->bloon_skin_moab));
    fn("bloon_skin_bfb", "std::string", fmt::format("{}", this->bloon_skin_bfb));
    fn("bloon_skin_zomg", "std::string", fmt::format("{}", this->bloon_skin_zomg));
    fn("bloon_skin_ddt", "std::string", fmt::format("{}", this->bloon_skin_ddt));
    fn("bloon_skin_bad", "std::string", fmt::format("{}", this->bloon_skin_bad));
    fn("common_vfx_pop", "std::string", fmt::format("{}", this->common_vfx_pop));
    fn("common_vfx_boost_tower", "std::string", fmt::format("{}", this->common_vfx_boost_tower));
    fn("common_vfx_boost_bloon", "std::string", fmt::format("{}", this->common_vfx_boost_bloon));

}


std::unique_ptr<GameEvent> SNetBlingLoadoutEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetBlingLoadoutEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->visual_emote_0);
    (void) reader.read(instance->visual_emote_1);
    (void) reader.read(instance->visual_emote_2);
    (void) reader.read(instance->visual_emote_3);
    (void) reader.read(instance->visual_emote_4);
    (void) reader.read(instance->visual_emote_5);
    (void) reader.read(instance->visual_emote_6);
    (void) reader.read(instance->visual_emote_7);
    (void) reader.read(instance->visual_emote_8);
    (void) reader.read(instance->visual_emote_9);
    (void) reader.read(instance->speech_emote_0);
    (void) reader.read(instance->speech_emote_1);
    (void) reader.read(instance->speech_emote_2);
    (void) reader.read(instance->speech_emote_3);
    (void) reader.read(instance->bloon_decal);
    (void) reader.read(instance->bloon_skin_moab);
    (void) reader.read(instance->bloon_skin_bfb);
    (void) reader.read(instance->bloon_skin_zomg);
    (void) reader.read(instance->bloon_skin_ddt);
    (void) reader.read(instance->bloon_skin_bad);
    (void) reader.read(instance->common_vfx_pop);
    (void) reader.read(instance->common_vfx_boost_tower);
    (void) reader.read(instance->common_vfx_boost_bloon);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetBlingLoadoutEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->visual_emote_0);
    (void) writer.write(this->visual_emote_1);
    (void) writer.write(this->visual_emote_2);
    (void) writer.write(this->visual_emote_3);
    (void) writer.write(this->visual_emote_4);
    (void) writer.write(this->visual_emote_5);
    (void) writer.write(this->visual_emote_6);
    (void) writer.write(this->visual_emote_7);
    (void) writer.write(this->visual_emote_8);
    (void) writer.write(this->visual_emote_9);
    (void) writer.write(this->speech_emote_0);
    (void) writer.write(this->speech_emote_1);
    (void) writer.write(this->speech_emote_2);
    (void) writer.write(this->speech_emote_3);
    (void) writer.write(this->bloon_decal);
    (void) writer.write(this->bloon_skin_moab);
    (void) writer.write(this->bloon_skin_bfb);
    (void) writer.write(this->bloon_skin_zomg);
    (void) writer.write(this->bloon_skin_ddt);
    (void) writer.write(this->bloon_skin_bad);
    (void) writer.write(this->common_vfx_pop);
    (void) writer.write(this->common_vfx_boost_tower);
    (void) writer.write(this->common_vfx_boost_bloon);


    return !writer.has_write_error();
}
            

SNetRequestSendTauntEvent::SNetRequestSendTauntEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetRequestSendTauntEvent::event_name() const {
    return "SNetRequestSendTauntEvent";
}


void SNetRequestSendTauntEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("taunt_index", "uint32_t", fmt::format("{}", this->taunt_index));

}


std::unique_ptr<GameEvent> SNetRequestSendTauntEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetRequestSendTauntEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->taunt_index);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetRequestSendTauntEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->taunt_index);


    return !writer.has_write_error();
}
            

SNetTickEvent::SNetTickEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetTickEvent::event_name() const {
    return "SNetTickEvent";
}


void SNetTickEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("flags", "unknown_16", fmt::format("{}", this->flags));
    fn("net_tick", "uint32_t", fmt::format("{}", this->net_tick));
    fn("sim_tick", "uint64_t", fmt::format("{}", this->sim_tick));
    fn("latency", "uint32_t", fmt::format("{}", this->latency));
    fn("jitter", "uint32_t", fmt::format("{}", this->jitter));
    fn("bloons_created", "uint64_t", fmt::format("{}", this->bloons_created));
    fn("bloons_leaked", "uint64_t", fmt::format("{}", this->bloons_leaked));
    fn("bloons_popped", "uint64_t", fmt::format("{}", this->bloons_popped));

}


std::unique_ptr<GameEvent> SNetTickEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetTickEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->flags);
    (void) reader.read(instance->net_tick);
    (void) reader.read(instance->sim_tick);
    (void) reader.read(instance->latency);
    (void) reader.read(instance->jitter);
    (void) reader.read(instance->bloons_created);
    (void) reader.read(instance->bloons_leaked);
    (void) reader.read(instance->bloons_popped);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetTickEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->flags);
    (void) writer.write(this->net_tick);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->latency);
    (void) writer.write(this->jitter);
    (void) writer.write(this->bloons_created);
    (void) writer.write(this->bloons_leaked);
    (void) writer.write(this->bloons_popped);


    return !writer.has_write_error();
}
            

SNetScheduleRoundStartEvent::SNetScheduleRoundStartEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetScheduleRoundStartEvent::event_name() const {
    return "SNetScheduleRoundStartEvent";
}


void SNetScheduleRoundStartEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("round_starts_at_tick", "uint64_t", fmt::format("{}", this->round_starts_at_tick));
    fn("round_idx", "uint32_t", fmt::format("{}", this->round_idx));
    fn("round_skip_prevention_enabled", "uint8_t", fmt::format("{}", this->round_skip_prevention_enabled));

}


std::unique_ptr<GameEvent> SNetScheduleRoundStartEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetScheduleRoundStartEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->round_starts_at_tick);
    (void) reader.read(instance->round_idx);
    (void) reader.read(instance->round_skip_prevention_enabled);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetScheduleRoundStartEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->round_starts_at_tick);
    (void) writer.write(this->round_idx);
    (void) writer.write(this->round_skip_prevention_enabled);


    return !writer.has_write_error();
}
            

SNetSetBadgesEvent::SNetSetBadgesEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetSetBadgesEvent::event_name() const {
    return "SNetSetBadgesEvent";
}


void SNetSetBadgesEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("badge_count", "uint8_t", fmt::format("{}", this->badge_count));

}


SNetInitPlayerResponseEvent::SNetInitPlayerResponseEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetInitPlayerResponseEvent::event_name() const {
    return "SNetInitPlayerResponseEvent";
}


void SNetInitPlayerResponseEvent::iter_fields(const iter_fields_callback_t &fn) const {

}


SNetSetBadgesEvent__Badge::SNetSetBadgesEvent__Badge(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetSetBadgesEvent__Badge::event_name() const {
    return "SNetSetBadgesEvent__Badge";
}


void SNetSetBadgesEvent__Badge::iter_fields(const iter_fields_callback_t &fn) const {
    fn("season_index", "uint8_t", fmt::format("{}", this->season_index));
    fn("type", "uint8_t", fmt::format("{}", this->type));

}


std::unique_ptr<GameEvent> SNetSetBadgesEvent__Badge::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetSetBadgesEvent__Badge>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->season_index);
    (void) reader.read(instance->type);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetSetBadgesEvent__Badge::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->season_index);
    (void) writer.write(this->type);


    return !writer.has_write_error();
}
            

SNetPatrolPointsSelectedEvent::SNetPatrolPointsSelectedEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetPatrolPointsSelectedEvent::event_name() const {
    return "SNetPatrolPointsSelectedEvent";
}


void SNetPatrolPointsSelectedEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("net_id", "uint64_t", fmt::format("{}", this->net_id));
    fn("net_id_version", "uint32_t", fmt::format("{}", this->net_id_version));
    fn("p0_x", "float_t", fmt::format("{}", this->p0_x));
    fn("p0_y", "float_t", fmt::format("{}", this->p0_y));
    fn("p1_x", "float_t", fmt::format("{}", this->p1_x));
    fn("p1_y", "float_t", fmt::format("{}", this->p1_y));

}


std::unique_ptr<GameEvent> SNetPatrolPointsSelectedEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetPatrolPointsSelectedEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->net_id);
    (void) reader.read(instance->net_id_version);
    (void) reader.read(instance->p0_x);
    (void) reader.read(instance->p0_y);
    (void) reader.read(instance->p1_x);
    (void) reader.read(instance->p1_y);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetPatrolPointsSelectedEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->net_id);
    (void) writer.write(this->net_id_version);
    (void) writer.write(this->p0_x);
    (void) writer.write(this->p0_y);
    (void) writer.write(this->p1_x);
    (void) writer.write(this->p1_y);


    return !writer.has_write_error();
}
            

SNetChangeTargetModeTowerEvent::SNetChangeTargetModeTowerEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetChangeTargetModeTowerEvent::event_name() const {
    return "SNetChangeTargetModeTowerEvent";
}


void SNetChangeTargetModeTowerEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("sim_tick", "uint64_t", fmt::format("{}", this->sim_tick));
    fn("target_mode", "uint32_t", fmt::format("{}", this->target_mode));
    fn("net_id", "uint64_t", fmt::format("{}", this->net_id));
    fn("net_id_version", "uint32_t", fmt::format("{}", this->net_id_version));

}


std::unique_ptr<GameEvent> SNetChangeTargetModeTowerEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetChangeTargetModeTowerEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->sim_tick);
    (void) reader.read(instance->target_mode);
    (void) reader.read(instance->net_id);
    (void) reader.read(instance->net_id_version);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetChangeTargetModeTowerEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->sim_tick);
    (void) writer.write(this->target_mode);
    (void) writer.write(this->net_id);
    (void) writer.write(this->net_id_version);


    return !writer.has_write_error();
}
            

SNetRequestSwitchFlyingDirectionEvent::SNetRequestSwitchFlyingDirectionEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetRequestSwitchFlyingDirectionEvent::event_name() const {
    return "SNetRequestSwitchFlyingDirectionEvent";
}


void SNetRequestSwitchFlyingDirectionEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("sim_tick", "uint64_t", fmt::format("{}", this->sim_tick));
    fn("net_id", "uint64_t", fmt::format("{}", this->net_id));
    fn("net_id_version", "uint32_t", fmt::format("{}", this->net_id_version));
    fn("is_flying_clockwise", "uint8_t", fmt::format("{}", this->is_flying_clockwise));

}


std::unique_ptr<GameEvent> SNetRequestSwitchFlyingDirectionEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetRequestSwitchFlyingDirectionEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->sim_tick);
    (void) reader.read(instance->net_id);
    (void) reader.read(instance->net_id_version);
    (void) reader.read(instance->is_flying_clockwise);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetRequestSwitchFlyingDirectionEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->sim_tick);
    (void) writer.write(this->net_id);
    (void) writer.write(this->net_id_version);
    (void) writer.write(this->is_flying_clockwise);


    return !writer.has_write_error();
}
            

SNetStartGameEvent::SNetStartGameEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetStartGameEvent::event_name() const {
    return "SNetStartGameEvent";
}


void SNetStartGameEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("first_round_tick", "uint64_t", fmt::format("{}", this->first_round_tick));

}


std::unique_ptr<GameEvent> SNetStartGameEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetStartGameEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->first_round_tick);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetStartGameEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->first_round_tick);


    return !writer.has_write_error();
}
            

SNetTowerLoadoutEvent::SNetTowerLoadoutEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetTowerLoadoutEvent::event_name() const {
    return "SNetTowerLoadoutEvent";
}


void SNetTowerLoadoutEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("hero_slot", "uint32_t", fmt::format("{}", this->hero_slot));
    fn("tower_slot_0", "uint32_t", fmt::format("{}", this->tower_slot_0));
    fn("tower_slot_1", "uint32_t", fmt::format("{}", this->tower_slot_1));
    fn("tower_slot_2", "uint32_t", fmt::format("{}", this->tower_slot_2));
    fn("hero_taunt_slot_0", "std::string", fmt::format("{}", this->hero_taunt_slot_0));
    fn("hero_taunt_slot_1", "std::string", fmt::format("{}", this->hero_taunt_slot_1));
    fn("hero_taunt_slot_2", "std::string", fmt::format("{}", this->hero_taunt_slot_2));
    fn("hero_tower_animations", "std::string", fmt::format("{}", this->hero_tower_animations));
    fn("tower_animations_0", "std::string", fmt::format("{}", this->tower_animations_0));
    fn("tower_animations_1", "std::string", fmt::format("{}", this->tower_animations_1));
    fn("tower_animations_2", "std::string", fmt::format("{}", this->tower_animations_2));
    fn("hero_vfx_loadout", "std::string", fmt::format("{}", this->hero_vfx_loadout));
    fn("tower_vfx_loadout_0", "std::string", fmt::format("{}", this->tower_vfx_loadout_0));
    fn("tower_vfx_loadout_1", "std::string", fmt::format("{}", this->tower_vfx_loadout_1));
    fn("tower_vfx_loadout_2", "std::string", fmt::format("{}", this->tower_vfx_loadout_2));
    fn("custom_name_0", "std::string", fmt::format("{}", this->custom_name_0));
    fn("custom_name_1", "std::string", fmt::format("{}", this->custom_name_1));
    fn("custom_name_2", "std::string", fmt::format("{}", this->custom_name_2));

}


std::unique_ptr<GameEvent> SNetTowerLoadoutEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetTowerLoadoutEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->hero_slot);
    (void) reader.read(instance->tower_slot_0);
    (void) reader.read(instance->tower_slot_1);
    (void) reader.read(instance->tower_slot_2);
    (void) reader.read(instance->hero_taunt_slot_0);
    (void) reader.read(instance->hero_taunt_slot_1);
    (void) reader.read(instance->hero_taunt_slot_2);
    (void) reader.read(instance->hero_tower_animations);
    (void) reader.read(instance->tower_animations_0);
    (void) reader.read(instance->tower_animations_1);
    (void) reader.read(instance->tower_animations_2);
    (void) reader.read(instance->hero_vfx_loadout);
    (void) reader.read(instance->tower_vfx_loadout_0);
    (void) reader.read(instance->tower_vfx_loadout_1);
    (void) reader.read(instance->tower_vfx_loadout_2);
    (void) reader.read(instance->custom_name_0);
    (void) reader.read(instance->custom_name_1);
    (void) reader.read(instance->custom_name_2);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetTowerLoadoutEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->hero_slot);
    (void) writer.write(this->tower_slot_0);
    (void) writer.write(this->tower_slot_1);
    (void) writer.write(this->tower_slot_2);
    (void) writer.write(this->hero_taunt_slot_0);
    (void) writer.write(this->hero_taunt_slot_1);
    (void) writer.write(this->hero_taunt_slot_2);
    (void) writer.write(this->hero_tower_animations);
    (void) writer.write(this->tower_animations_0);
    (void) writer.write(this->tower_animations_1);
    (void) writer.write(this->tower_animations_2);
    (void) writer.write(this->hero_vfx_loadout);
    (void) writer.write(this->tower_vfx_loadout_0);
    (void) writer.write(this->tower_vfx_loadout_1);
    (void) writer.write(this->tower_vfx_loadout_2);
    (void) writer.write(this->custom_name_0);
    (void) writer.write(this->custom_name_1);
    (void) writer.write(this->custom_name_2);


    return !writer.has_write_error();
}
            

SNetSendSurrenderNetEvent::SNetSendSurrenderNetEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetSendSurrenderNetEvent::event_name() const {
    return "SNetSendSurrenderNetEvent";
}


void SNetSendSurrenderNetEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("tick", "uint64_t", fmt::format("{}", this->tick));
    fn("side", "uint8_t", fmt::format("{}", this->side));

}


std::unique_ptr<GameEvent> SNetSendSurrenderNetEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetSendSurrenderNetEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->tick);
    (void) reader.read(instance->side);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetSendSurrenderNetEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->tick);
    (void) writer.write(this->side);


    return !writer.has_write_error();
}
            

SNetInitPlayerPostponedEvent::SNetInitPlayerPostponedEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetInitPlayerPostponedEvent::event_name() const {
    return "SNetInitPlayerPostponedEvent";
}


void SNetInitPlayerPostponedEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("due_to_missing_assets_on_referee", "uint8_t", fmt::format("{}", this->due_to_missing_assets_on_referee));
    fn("flags", "uint8_t", fmt::format("{}", this->flags));

}


std::unique_ptr<GameEvent> SNetInitPlayerPostponedEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetInitPlayerPostponedEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->due_to_missing_assets_on_referee);
    (void) reader.read(instance->flags);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetInitPlayerPostponedEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->due_to_missing_assets_on_referee);
    (void) writer.write(this->flags);


    return !writer.has_write_error();
}
            

SNetLastRoundGameEndRequestEvent::SNetLastRoundGameEndRequestEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetLastRoundGameEndRequestEvent::event_name() const {
    return "SNetLastRoundGameEndRequestEvent";
}


void SNetLastRoundGameEndRequestEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("side", "uint8_t", fmt::format("{}", this->side));
    fn("please_end_the_game", "uint8_t", fmt::format("{}", this->please_end_the_game));
    fn("sim_tick", "uint64_t", fmt::format("{}", this->sim_tick));

}


std::unique_ptr<GameEvent> SNetLastRoundGameEndRequestEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetLastRoundGameEndRequestEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->side);
    (void) reader.read(instance->please_end_the_game);
    (void) reader.read(instance->sim_tick);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetLastRoundGameEndRequestEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->side);
    (void) writer.write(this->please_end_the_game);
    (void) writer.write(this->sim_tick);


    return !writer.has_write_error();
}
            

SNetRequestFireWeaponEvent::SNetRequestFireWeaponEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetRequestFireWeaponEvent::event_name() const {
    return "SNetRequestFireWeaponEvent";
}


void SNetRequestFireWeaponEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("sim_tick", "uint64_t", fmt::format("{}", this->sim_tick));
    fn("net_id", "uint64_t", fmt::format("{}", this->net_id));
    fn("net_id_version", "uint32_t", fmt::format("{}", this->net_id_version));
    fn("eco_receipt_id", "uint64_t", fmt::format("{}", this->eco_receipt_id));

}


SNetMoveToTowerSelectEvent::SNetMoveToTowerSelectEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetMoveToTowerSelectEvent::event_name() const {
    return "SNetMoveToTowerSelectEvent";
}


void SNetMoveToTowerSelectEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("lobby_ready_countdown", "float_t", fmt::format("{}", this->lobby_ready_countdown));

}


std::unique_ptr<GameEvent> SNetMoveToTowerSelectEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetMoveToTowerSelectEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->lobby_ready_countdown);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetMoveToTowerSelectEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->lobby_ready_countdown);


    return !writer.has_write_error();
}
            

SNetDidFirePlayerAbilityEvent::SNetDidFirePlayerAbilityEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetDidFirePlayerAbilityEvent::event_name() const {
    return "SNetDidFirePlayerAbilityEvent";
}


void SNetDidFirePlayerAbilityEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("sim_tick", "uint64_t", fmt::format("{}", this->sim_tick));
    fn("ability_type", "uint32_t", fmt::format("{}", this->ability_type));

}


std::unique_ptr<GameEvent> SNetDidFirePlayerAbilityEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetDidFirePlayerAbilityEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->sim_tick);
    (void) reader.read(instance->ability_type);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetDidFirePlayerAbilityEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->sim_tick);
    (void) writer.write(this->ability_type);


    return !writer.has_write_error();
}
            

SNetTargetTouchPointEvent::SNetTargetTouchPointEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetTargetTouchPointEvent::event_name() const {
    return "SNetTargetTouchPointEvent";
}


void SNetTargetTouchPointEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("sim_tick", "uint64_t", fmt::format("{}", this->sim_tick));
    fn("point_x", "float_t", fmt::format("{}", this->point_x));
    fn("point_y", "float_t", fmt::format("{}", this->point_y));
    fn("released", "uint8_t", fmt::format("{}", this->released));

}


std::unique_ptr<GameEvent> SNetTargetTouchPointEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetTargetTouchPointEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->sim_tick);
    (void) reader.read(instance->point_x);
    (void) reader.read(instance->point_y);
    (void) reader.read(instance->released);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetTargetTouchPointEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->sim_tick);
    (void) writer.write(this->point_x);
    (void) writer.write(this->point_y);
    (void) writer.write(this->released);


    return !writer.has_write_error();
}
            

SNetAssetBlobUploadEvent::SNetAssetBlobUploadEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetAssetBlobUploadEvent::event_name() const {
    return "SNetAssetBlobUploadEvent";
}


void SNetAssetBlobUploadEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("asset_type", "uint64_t", fmt::format("{}", this->asset_type));
    fn("data", "std::vector<uint8_t>", fmt::format("{}", fmt::join(this->data, ", ")));

}


SNetBombBlitzActivatedEvent::SNetBombBlitzActivatedEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetBombBlitzActivatedEvent::event_name() const {
    return "SNetBombBlitzActivatedEvent";
}


void SNetBombBlitzActivatedEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("sim_tick", "uint64_t", fmt::format("{}", this->sim_tick));
    fn("activate_tick", "uint64_t", fmt::format("{}", this->activate_tick));
    fn("deactivate_tick", "uint64_t", fmt::format("{}", this->deactivate_tick));

}


std::unique_ptr<GameEvent> SNetBombBlitzActivatedEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetBombBlitzActivatedEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->sim_tick);
    (void) reader.read(instance->activate_tick);
    (void) reader.read(instance->deactivate_tick);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetBombBlitzActivatedEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->sim_tick);
    (void) writer.write(this->activate_tick);
    (void) writer.write(this->deactivate_tick);


    return !writer.has_write_error();
}
            

SNetCenterPointSelectedEvent::SNetCenterPointSelectedEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetCenterPointSelectedEvent::event_name() const {
    return "SNetCenterPointSelectedEvent";
}


void SNetCenterPointSelectedEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("net_id", "uint64_t", fmt::format("{}", this->net_id));
    fn("net_id_version", "uint32_t", fmt::format("{}", this->net_id_version));
    fn("p0_x", "float_t", fmt::format("{}", this->p0_x));
    fn("p0_y", "float_t", fmt::format("{}", this->p0_y));

}


std::unique_ptr<GameEvent> SNetCenterPointSelectedEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetCenterPointSelectedEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->net_id);
    (void) reader.read(instance->net_id_version);
    (void) reader.read(instance->p0_x);
    (void) reader.read(instance->p0_y);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetCenterPointSelectedEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->net_id);
    (void) writer.write(this->net_id_version);
    (void) writer.write(this->p0_x);
    (void) writer.write(this->p0_y);


    return !writer.has_write_error();
}
            

SNetCollectablesCollectedEvent::SNetCollectablesCollectedEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetCollectablesCollectedEvent::event_name() const {
    return "SNetCollectablesCollectedEvent";
}


void SNetCollectablesCollectedEvent::iter_fields(const iter_fields_callback_t &fn) const {

}


SNetConfirmRoundStartEvent::SNetConfirmRoundStartEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetConfirmRoundStartEvent::event_name() const {
    return "SNetConfirmRoundStartEvent";
}


void SNetConfirmRoundStartEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("round_starts_at_tick", "uint64_t", fmt::format("{}", this->round_starts_at_tick));
    fn("round_idx", "uint32_t", fmt::format("{}", this->round_idx));

}


std::unique_ptr<GameEvent> SNetConfirmRoundStartEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetConfirmRoundStartEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->round_starts_at_tick);
    (void) reader.read(instance->round_idx);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetConfirmRoundStartEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->round_starts_at_tick);
    (void) writer.write(this->round_idx);


    return !writer.has_write_error();
}
            

SNetDebug_LoadScenarioEvent::SNetDebug_LoadScenarioEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetDebug_LoadScenarioEvent::event_name() const {
    return "SNetDebug_LoadScenarioEvent";
}


void SNetDebug_LoadScenarioEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("scenario_number", "uint32_t", fmt::format("{}", this->scenario_number));

}


std::unique_ptr<GameEvent> SNetDebug_LoadScenarioEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetDebug_LoadScenarioEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->scenario_number);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetDebug_LoadScenarioEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->scenario_number);


    return !writer.has_write_error();
}
            

SNetDebug_SetSimSpeedEvent::SNetDebug_SetSimSpeedEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetDebug_SetSimSpeedEvent::event_name() const {
    return "SNetDebug_SetSimSpeedEvent";
}


void SNetDebug_SetSimSpeedEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("speed", "float_t", fmt::format("{}", this->speed));

}


std::unique_ptr<GameEvent> SNetDebug_SetSimSpeedEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetDebug_SetSimSpeedEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->speed);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetDebug_SetSimSpeedEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->speed);


    return !writer.has_write_error();
}
            

SNetDebug_StartRoundEvent::SNetDebug_StartRoundEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetDebug_StartRoundEvent::event_name() const {
    return "SNetDebug_StartRoundEvent";
}


void SNetDebug_StartRoundEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("round_starts_at_tick", "uint64_t", fmt::format("{}", this->round_starts_at_tick));
    fn("round_idx", "uint32_t", fmt::format("{}", this->round_idx));

}


std::unique_ptr<GameEvent> SNetDebug_StartRoundEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetDebug_StartRoundEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->round_starts_at_tick);
    (void) reader.read(instance->round_idx);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetDebug_StartRoundEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->round_starts_at_tick);
    (void) writer.write(this->round_idx);


    return !writer.has_write_error();
}
            

SNetInitPlayerRequestEvent::SNetInitPlayerRequestEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetInitPlayerRequestEvent::event_name() const {
    return "SNetInitPlayerRequestEvent";
}


void SNetInitPlayerRequestEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("rng_seed", "uint32_t", fmt::format("{}", this->rng_seed));
    fn("player_ids_by_side", "std::vector<uint8_t>", fmt::format("{}", fmt::join(this->player_ids_by_side, ", ")));
    fn("lobby_generation", "uint16_t", fmt::format("{}", this->lobby_generation));

}


std::unique_ptr<GameEvent> SNetInitPlayerRequestEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetInitPlayerRequestEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->rng_seed);
    (void) reader.read(instance->player_ids_by_side);
    (void) reader.read(instance->lobby_generation);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetInitPlayerRequestEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->rng_seed);
    (void) writer.write(this->player_ids_by_side);
    (void) writer.write(this->lobby_generation);


    return !writer.has_write_error();
}
            

SNetDidQueueBloonSetEvent::SNetDidQueueBloonSetEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetDidQueueBloonSetEvent::event_name() const {
    return "SNetDidQueueBloonSetEvent";
}


void SNetDidQueueBloonSetEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("sim_tick", "uint64_t", fmt::format("{}", this->sim_tick));
    fn("bloon_set_id", "uint32_t", fmt::format("{}", this->bloon_set_id));
    fn("bloon_features", "uint16_t", fmt::format("{}", this->bloon_features));
    fn("path_index", "uint32_t", fmt::format("{}", this->path_index));

}


std::unique_ptr<GameEvent> SNetDidQueueBloonSetEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetDidQueueBloonSetEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->sim_tick);
    (void) reader.read(instance->bloon_set_id);
    (void) reader.read(instance->bloon_features);
    (void) reader.read(instance->path_index);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetDidQueueBloonSetEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->sim_tick);
    (void) writer.write(this->bloon_set_id);
    (void) writer.write(this->bloon_features);
    (void) writer.write(this->path_index);


    return !writer.has_write_error();
}
            

SNetEcoUpdatedEvent::SNetEcoUpdatedEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetEcoUpdatedEvent::event_name() const {
    return "SNetEcoUpdatedEvent";
}


void SNetEcoUpdatedEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("cash", "float_t", fmt::format("{}", this->cash));
    fn("income", "float_t", fmt::format("{}", this->income));
    fn("eco_receipt_id", "uint64_t", fmt::format("{}", this->eco_receipt_id));
    fn("context", "unknown_8", fmt::format("{}", this->context));
    fn("subcontext", "uint64_t", fmt::format("{}", this->subcontext));

}


std::unique_ptr<GameEvent> SNetEcoUpdatedEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetEcoUpdatedEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->cash);
    (void) reader.read(instance->income);
    (void) reader.read(instance->eco_receipt_id);
    (void) reader.read(instance->context);
    (void) reader.read(instance->subcontext);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetEcoUpdatedEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->cash);
    (void) writer.write(this->income);
    (void) writer.write(this->eco_receipt_id);
    (void) writer.write(this->context);
    (void) writer.write(this->subcontext);


    return !writer.has_write_error();
}
            

SNetFirePlayerAbilityEvent::SNetFirePlayerAbilityEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetFirePlayerAbilityEvent::event_name() const {
    return "SNetFirePlayerAbilityEvent";
}


void SNetFirePlayerAbilityEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("sim_tick", "uint64_t", fmt::format("{}", this->sim_tick));
    fn("ability_type", "uint32_t", fmt::format("{}", this->ability_type));

}


std::unique_ptr<GameEvent> SNetFirePlayerAbilityEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetFirePlayerAbilityEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->sim_tick);
    (void) reader.read(instance->ability_type);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetFirePlayerAbilityEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->sim_tick);
    (void) writer.write(this->ability_type);


    return !writer.has_write_error();
}
            

SNetInitPlayerCompleteEvent::SNetInitPlayerCompleteEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetInitPlayerCompleteEvent::event_name() const {
    return "SNetInitPlayerCompleteEvent";
}


void SNetInitPlayerCompleteEvent::iter_fields(const iter_fields_callback_t &fn) const {

}


std::unique_ptr<GameEvent> SNetInitPlayerCompleteEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetInitPlayerCompleteEvent>(event_id, sim_tick, unknown_u8);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetInitPlayerCompleteEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);



    return !writer.has_write_error();
}
            

SNetLobbyErrorEvent::SNetLobbyErrorEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetLobbyErrorEvent::event_name() const {
    return "SNetLobbyErrorEvent";
}


void SNetLobbyErrorEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("error_code", "unknown_32", fmt::format("{}", this->error_code));

}


std::unique_ptr<GameEvent> SNetLobbyErrorEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetLobbyErrorEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->error_code);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetLobbyErrorEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->error_code);


    return !writer.has_write_error();
}
            

SNetLockInPlacePointSelectedEvent::SNetLockInPlacePointSelectedEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetLockInPlacePointSelectedEvent::event_name() const {
    return "SNetLockInPlacePointSelectedEvent";
}


void SNetLockInPlacePointSelectedEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("net_id", "uint64_t", fmt::format("{}", this->net_id));
    fn("net_id_version", "uint32_t", fmt::format("{}", this->net_id_version));
    fn("p0_x", "float_t", fmt::format("{}", this->p0_x));
    fn("p0_y", "float_t", fmt::format("{}", this->p0_y));

}


std::unique_ptr<GameEvent> SNetLockInPlacePointSelectedEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetLockInPlacePointSelectedEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->net_id);
    (void) reader.read(instance->net_id_version);
    (void) reader.read(instance->p0_x);
    (void) reader.read(instance->p0_y);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetLockInPlacePointSelectedEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->net_id);
    (void) writer.write(this->net_id_version);
    (void) writer.write(this->p0_x);
    (void) writer.write(this->p0_y);


    return !writer.has_write_error();
}
            

SNetReadyToPlayEvent::SNetReadyToPlayEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetReadyToPlayEvent::event_name() const {
    return "SNetReadyToPlayEvent";
}


void SNetReadyToPlayEvent::iter_fields(const iter_fields_callback_t &fn) const {

}


std::unique_ptr<GameEvent> SNetReadyToPlayEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetReadyToPlayEvent>(event_id, sim_tick, unknown_u8);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetReadyToPlayEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);



    return !writer.has_write_error();
}
            

SNetReadyUpEvent::SNetReadyUpEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetReadyUpEvent::event_name() const {
    return "SNetReadyUpEvent";
}


void SNetReadyUpEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("ready_phase", "uint16_t", fmt::format("{}", this->ready_phase));
    fn("lobby_generation", "uint16_t", fmt::format("{}", this->lobby_generation));

}


std::unique_ptr<GameEvent> SNetReadyUpEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetReadyUpEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->ready_phase);
    (void) reader.read(instance->lobby_generation);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetReadyUpEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->ready_phase);
    (void) writer.write(this->lobby_generation);


    return !writer.has_write_error();
}
            

SNetRefereeSelectedTowerLoadoutEvent::SNetRefereeSelectedTowerLoadoutEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetRefereeSelectedTowerLoadoutEvent::event_name() const {
    return "SNetRefereeSelectedTowerLoadoutEvent";
}


void SNetRefereeSelectedTowerLoadoutEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("hero_slot", "uint32_t", fmt::format("{}", this->hero_slot));
    fn("tower_slot_0", "uint32_t", fmt::format("{}", this->tower_slot_0));
    fn("tower_slot_1", "uint32_t", fmt::format("{}", this->tower_slot_1));
    fn("tower_slot_2", "uint32_t", fmt::format("{}", this->tower_slot_2));
    fn("flags", "uint8_t", fmt::format("{}", this->flags));

}


std::unique_ptr<GameEvent> SNetRefereeSelectedTowerLoadoutEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetRefereeSelectedTowerLoadoutEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->hero_slot);
    (void) reader.read(instance->tower_slot_0);
    (void) reader.read(instance->tower_slot_1);
    (void) reader.read(instance->tower_slot_2);
    (void) reader.read(instance->flags);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetRefereeSelectedTowerLoadoutEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->hero_slot);
    (void) writer.write(this->tower_slot_0);
    (void) writer.write(this->tower_slot_1);
    (void) writer.write(this->tower_slot_2);
    (void) writer.write(this->flags);


    return !writer.has_write_error();
}
            

SNetRemoveBlockerEvent::SNetRemoveBlockerEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetRemoveBlockerEvent::event_name() const {
    return "SNetRemoveBlockerEvent";
}


void SNetRemoveBlockerEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("sim_tick", "uint64_t", fmt::format("{}", this->sim_tick));
    fn("blocker_group", "uint64_t", fmt::format("{}", this->blocker_group));
    fn("eco_receipt_id", "uint64_t", fmt::format("{}", this->eco_receipt_id));

}


std::unique_ptr<GameEvent> SNetRemoveBlockerEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetRemoveBlockerEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->sim_tick);
    (void) reader.read(instance->blocker_group);
    (void) reader.read(instance->eco_receipt_id);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetRemoveBlockerEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->sim_tick);
    (void) writer.write(this->blocker_group);
    (void) writer.write(this->eco_receipt_id);


    return !writer.has_write_error();
}
            

SNetRemoveTowerEvent::SNetRemoveTowerEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetRemoveTowerEvent::event_name() const {
    return "SNetRemoveTowerEvent";
}


void SNetRemoveTowerEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("sim_tick", "uint64_t", fmt::format("{}", this->sim_tick));
    fn("net_id", "uint64_t", fmt::format("{}", this->net_id));
    fn("net_id_version", "uint32_t", fmt::format("{}", this->net_id_version));
    fn("tower_type", "uint32_t", fmt::format("{}", this->tower_type));
    fn("tower_upgrade_level", "uint32_t", fmt::format("{}", this->tower_upgrade_level));
    fn("eco_receipt_id", "uint64_t", fmt::format("{}", this->eco_receipt_id));

}


std::unique_ptr<GameEvent> SNetRemoveTowerEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetRemoveTowerEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->sim_tick);
    (void) reader.read(instance->net_id);
    (void) reader.read(instance->net_id_version);
    (void) reader.read(instance->tower_type);
    (void) reader.read(instance->tower_upgrade_level);
    (void) reader.read(instance->eco_receipt_id);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetRemoveTowerEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->sim_tick);
    (void) writer.write(this->net_id);
    (void) writer.write(this->net_id_version);
    (void) writer.write(this->tower_type);
    (void) writer.write(this->tower_upgrade_level);
    (void) writer.write(this->eco_receipt_id);


    return !writer.has_write_error();
}
            

SNetRequestCurrentHealthEvent::SNetRequestCurrentHealthEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetRequestCurrentHealthEvent::event_name() const {
    return "SNetRequestCurrentHealthEvent";
}


void SNetRequestCurrentHealthEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("tick", "uint64_t", fmt::format("{}", this->tick));

}


std::unique_ptr<GameEvent> SNetRequestCurrentHealthEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetRequestCurrentHealthEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->tick);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetRequestCurrentHealthEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->tick);


    return !writer.has_write_error();
}
            

SNetRequestQueueBloonSetEvent::SNetRequestQueueBloonSetEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetRequestQueueBloonSetEvent::event_name() const {
    return "SNetRequestQueueBloonSetEvent";
}


void SNetRequestQueueBloonSetEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("target_side", "uint8_t", fmt::format("{}", this->target_side));
    fn("sim_tick", "uint64_t", fmt::format("{}", this->sim_tick));
    fn("bloon_set_id", "uint32_t", fmt::format("{}", this->bloon_set_id));
    fn("bloon_features", "uint16_t", fmt::format("{}", this->bloon_features));
    fn("path_index", "uint32_t", fmt::format("{}", this->path_index));
    fn("cost", "float_t", fmt::format("{}", this->cost));
    fn("income_change", "float_t", fmt::format("{}", this->income_change));
    fn("eco_receipt_id", "uint64_t", fmt::format("{}", this->eco_receipt_id));
    fn("self_send_flag", "uint8_t", fmt::format("{}", this->self_send_flag));

}


std::unique_ptr<GameEvent> SNetRequestQueueBloonSetEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetRequestQueueBloonSetEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->target_side);
    (void) reader.read(instance->sim_tick);
    (void) reader.read(instance->bloon_set_id);
    (void) reader.read(instance->bloon_features);
    (void) reader.read(instance->path_index);
    (void) reader.read(instance->cost);
    (void) reader.read(instance->income_change);
    (void) reader.read(instance->eco_receipt_id);
    (void) reader.read(instance->self_send_flag);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetRequestQueueBloonSetEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->target_side);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->bloon_set_id);
    (void) writer.write(this->bloon_features);
    (void) writer.write(this->path_index);
    (void) writer.write(this->cost);
    (void) writer.write(this->income_change);
    (void) writer.write(this->eco_receipt_id);
    (void) writer.write(this->self_send_flag);


    return !writer.has_write_error();
}
            

SNetRequestSetTowerTargetPointEvent::SNetRequestSetTowerTargetPointEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetRequestSetTowerTargetPointEvent::event_name() const {
    return "SNetRequestSetTowerTargetPointEvent";
}


void SNetRequestSetTowerTargetPointEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("sim_tick", "uint64_t", fmt::format("{}", this->sim_tick));
    fn("net_id", "uint64_t", fmt::format("{}", this->net_id));
    fn("net_id_version", "uint32_t", fmt::format("{}", this->net_id_version));
    fn("pos_x", "float_t", fmt::format("{}", this->pos_x));
    fn("pos_y", "float_t", fmt::format("{}", this->pos_y));

}


std::unique_ptr<GameEvent> SNetRequestSetTowerTargetPointEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetRequestSetTowerTargetPointEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->sim_tick);
    (void) reader.read(instance->net_id);
    (void) reader.read(instance->net_id_version);
    (void) reader.read(instance->pos_x);
    (void) reader.read(instance->pos_y);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetRequestSetTowerTargetPointEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->sim_tick);
    (void) writer.write(this->net_id);
    (void) writer.write(this->net_id_version);
    (void) writer.write(this->pos_x);
    (void) writer.write(this->pos_y);


    return !writer.has_write_error();
}
            

SNetRequestSimSnapshotEvent::SNetRequestSimSnapshotEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetRequestSimSnapshotEvent::event_name() const {
    return "SNetRequestSimSnapshotEvent";
}


void SNetRequestSimSnapshotEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("take_snapshot_at_tick", "uint64_t", fmt::format("{}", this->take_snapshot_at_tick));

}


std::unique_ptr<GameEvent> SNetRequestSimSnapshotEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetRequestSimSnapshotEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->take_snapshot_at_tick);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetRequestSimSnapshotEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->take_snapshot_at_tick);


    return !writer.has_write_error();
}
            

SNetRequestStartTowerFlightPathRejoinEvent::SNetRequestStartTowerFlightPathRejoinEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetRequestStartTowerFlightPathRejoinEvent::event_name() const {
    return "SNetRequestStartTowerFlightPathRejoinEvent";
}


void SNetRequestStartTowerFlightPathRejoinEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("sim_tick", "uint64_t", fmt::format("{}", this->sim_tick));
    fn("net_id", "uint64_t", fmt::format("{}", this->net_id));
    fn("net_id_version", "uint32_t", fmt::format("{}", this->net_id_version));
    fn("start_path_tick", "uint64_t", fmt::format("{}", this->start_path_tick));
    fn("end_path_tick", "uint64_t", fmt::format("{}", this->end_path_tick));
    fn("p0_x", "float_t", fmt::format("{}", this->p0_x));
    fn("p0_y", "float_t", fmt::format("{}", this->p0_y));
    fn("p1_x", "float_t", fmt::format("{}", this->p1_x));
    fn("p1_y", "float_t", fmt::format("{}", this->p1_y));
    fn("p2_x", "float_t", fmt::format("{}", this->p2_x));
    fn("p2_y", "float_t", fmt::format("{}", this->p2_y));
    fn("p3_x", "float_t", fmt::format("{}", this->p3_x));
    fn("p3_y", "float_t", fmt::format("{}", this->p3_y));

}


std::unique_ptr<GameEvent> SNetRequestStartTowerFlightPathRejoinEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetRequestStartTowerFlightPathRejoinEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->sim_tick);
    (void) reader.read(instance->net_id);
    (void) reader.read(instance->net_id_version);
    (void) reader.read(instance->start_path_tick);
    (void) reader.read(instance->end_path_tick);
    (void) reader.read(instance->p0_x);
    (void) reader.read(instance->p0_y);
    (void) reader.read(instance->p1_x);
    (void) reader.read(instance->p1_y);
    (void) reader.read(instance->p2_x);
    (void) reader.read(instance->p2_y);
    (void) reader.read(instance->p3_x);
    (void) reader.read(instance->p3_y);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetRequestStartTowerFlightPathRejoinEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->sim_tick);
    (void) writer.write(this->net_id);
    (void) writer.write(this->net_id_version);
    (void) writer.write(this->start_path_tick);
    (void) writer.write(this->end_path_tick);
    (void) writer.write(this->p0_x);
    (void) writer.write(this->p0_y);
    (void) writer.write(this->p1_x);
    (void) writer.write(this->p1_y);
    (void) writer.write(this->p2_x);
    (void) writer.write(this->p2_y);
    (void) writer.write(this->p3_x);
    (void) writer.write(this->p3_y);


    return !writer.has_write_error();
}
            

SNetRequestSwitchHandEvent::SNetRequestSwitchHandEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetRequestSwitchHandEvent::event_name() const {
    return "SNetRequestSwitchHandEvent";
}


void SNetRequestSwitchHandEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("sim_tick", "uint64_t", fmt::format("{}", this->sim_tick));
    fn("net_id", "uint64_t", fmt::format("{}", this->net_id));
    fn("net_id_version", "uint32_t", fmt::format("{}", this->net_id_version));
    fn("is_left_handed", "uint8_t", fmt::format("{}", this->is_left_handed));

}


std::unique_ptr<GameEvent> SNetRequestSwitchHandEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetRequestSwitchHandEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->sim_tick);
    (void) reader.read(instance->net_id);
    (void) reader.read(instance->net_id_version);
    (void) reader.read(instance->is_left_handed);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetRequestSwitchHandEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->sim_tick);
    (void) writer.write(this->net_id);
    (void) writer.write(this->net_id_version);
    (void) writer.write(this->is_left_handed);


    return !writer.has_write_error();
}
            

SNetSendLifeChangedEvent::SNetSendLifeChangedEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetSendLifeChangedEvent::event_name() const {
    return "SNetSendLifeChangedEvent";
}


void SNetSendLifeChangedEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("tick", "uint64_t", fmt::format("{}", this->tick));
    fn("life_before", "float_t", fmt::format("{}", this->life_before));
    fn("life_after", "float_t", fmt::format("{}", this->life_after));

}


std::unique_ptr<GameEvent> SNetSendLifeChangedEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetSendLifeChangedEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->tick);
    (void) reader.read(instance->life_before);
    (void) reader.read(instance->life_after);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetSendLifeChangedEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->tick);
    (void) writer.write(this->life_before);
    (void) writer.write(this->life_after);


    return !writer.has_write_error();
}
            

SNetSetGameMapEvent::SNetSetGameMapEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetSetGameMapEvent::event_name() const {
    return "SNetSetGameMapEvent";
}


void SNetSetGameMapEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("map_asset_id", "uint64_t", fmt::format("{}", this->map_asset_id));

}


std::unique_ptr<GameEvent> SNetSetGameMapEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetSetGameMapEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->map_asset_id);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetSetGameMapEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->map_asset_id);


    return !writer.has_write_error();
}
            

SNetSetNameEvent::SNetSetNameEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetSetNameEvent::event_name() const {
    return "SNetSetNameEvent";
}


void SNetSetNameEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("is_vip", "uint8_t", fmt::format("{}", this->is_vip));
    fn("avatar_id", "std::string", fmt::format("{}", this->avatar_id));
    fn("banner_id", "std::string", fmt::format("{}", this->banner_id));
    fn("border_id", "std::string", fmt::format("{}", this->border_id));
    fn("nkapi_id", "std::string", fmt::format("{}", this->nkapi_id));
    fn("can_do_master_match", "uint8_t", fmt::format("{}", this->can_do_master_match));

}


std::unique_ptr<GameEvent> SNetSetNameEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetSetNameEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->is_vip);
    (void) reader.read(instance->avatar_id);
    (void) reader.read(instance->banner_id);
    (void) reader.read(instance->border_id);
    (void) reader.read(instance->nkapi_id);
    (void) reader.read(instance->can_do_master_match);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetSetNameEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->is_vip);
    (void) writer.write(this->avatar_id);
    (void) writer.write(this->banner_id);
    (void) writer.write(this->border_id);
    (void) writer.write(this->nkapi_id);
    (void) writer.write(this->can_do_master_match);


    return !writer.has_write_error();
}
            

SNetSimSnapshotEvent::SNetSimSnapshotEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetSimSnapshotEvent::event_name() const {
    return "SNetSimSnapshotEvent";
}


void SNetSimSnapshotEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("tick", "uint64_t", fmt::format("{}", this->tick));
    fn("data", "std::vector<uint8_t>", fmt::format("{}", fmt::join(this->data, ", ")));

}


std::unique_ptr<GameEvent> SNetSimSnapshotEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetSimSnapshotEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->tick);
    (void) reader.read(instance->data);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetSimSnapshotEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->tick);
    (void) writer.write(this->data);


    return !writer.has_write_error();
}
            

SNetStartLoadingEvent::SNetStartLoadingEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetStartLoadingEvent::event_name() const {
    return "SNetStartLoadingEvent";
}


void SNetStartLoadingEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("map", "uint64_t", fmt::format("{}", this->map));
    fn("left_hero_slot", "uint32_t", fmt::format("{}", this->left_hero_slot));
    fn("left_tower_slot_0", "uint32_t", fmt::format("{}", this->left_tower_slot_0));
    fn("left_tower_slot_1", "uint32_t", fmt::format("{}", this->left_tower_slot_1));
    fn("left_tower_slot_2", "uint32_t", fmt::format("{}", this->left_tower_slot_2));
    fn("right_hero_slot", "uint32_t", fmt::format("{}", this->right_hero_slot));
    fn("right_tower_slot_0", "uint32_t", fmt::format("{}", this->right_tower_slot_0));
    fn("right_tower_slot_1", "uint32_t", fmt::format("{}", this->right_tower_slot_1));
    fn("right_tower_slot_2", "uint32_t", fmt::format("{}", this->right_tower_slot_2));
    fn("result", "uint8_t", fmt::format("{}", this->result));

}


std::unique_ptr<GameEvent> SNetStartLoadingEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetStartLoadingEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->map);
    (void) reader.read(instance->left_hero_slot);
    (void) reader.read(instance->left_tower_slot_0);
    (void) reader.read(instance->left_tower_slot_1);
    (void) reader.read(instance->left_tower_slot_2);
    (void) reader.read(instance->right_hero_slot);
    (void) reader.read(instance->right_tower_slot_0);
    (void) reader.read(instance->right_tower_slot_1);
    (void) reader.read(instance->right_tower_slot_2);
    (void) reader.read(instance->result);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetStartLoadingEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->map);
    (void) writer.write(this->left_hero_slot);
    (void) writer.write(this->left_tower_slot_0);
    (void) writer.write(this->left_tower_slot_1);
    (void) writer.write(this->left_tower_slot_2);
    (void) writer.write(this->right_hero_slot);
    (void) writer.write(this->right_tower_slot_0);
    (void) writer.write(this->right_tower_slot_1);
    (void) writer.write(this->right_tower_slot_2);
    (void) writer.write(this->result);


    return !writer.has_write_error();
}
            

SNetStartLobbyEvent::SNetStartLobbyEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetStartLobbyEvent::event_name() const {
    return "SNetStartLobbyEvent";
}


void SNetStartLobbyEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("lobby_clock", "uint8_t", fmt::format("{}", this->lobby_clock));
    fn("map_asset_id", "uint64_t", fmt::format("{}", this->map_asset_id));

}


std::unique_ptr<GameEvent> SNetStartLobbyEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetStartLobbyEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->lobby_clock);
    (void) reader.read(instance->map_asset_id);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetStartLobbyEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->lobby_clock);
    (void) writer.write(this->map_asset_id);


    return !writer.has_write_error();
}
            

SNetUpgradeTowerEvent::SNetUpgradeTowerEvent(uint16_t event_id, uint64_t unknown_64, uint8_t unknown_u8)
    : GameEvent(event_id, unknown_64, unknown_u8) { }

std::string SNetUpgradeTowerEvent::event_name() const {
    return "SNetUpgradeTowerEvent";
}


void SNetUpgradeTowerEvent::iter_fields(const iter_fields_callback_t &fn) const {
    fn("sim_tick", "uint64_t", fmt::format("{}", this->sim_tick));
    fn("path", "uint8_t", fmt::format("{}", this->path));
    fn("to_level", "int8_t", fmt::format("{}", this->to_level));
    fn("cost", "float_t", fmt::format("{}", this->cost));
    fn("cost_xp", "float_t", fmt::format("{}", this->cost_xp));
    fn("net_id", "uint64_t", fmt::format("{}", this->net_id));
    fn("net_id_version", "uint32_t", fmt::format("{}", this->net_id_version));
    fn("tower_type", "uint32_t", fmt::format("{}", this->tower_type));
    fn("eco_receipt_id", "uint64_t", fmt::format("{}", this->eco_receipt_id));

}


std::unique_ptr<GameEvent> SNetUpgradeTowerEvent::decode(const NetworkMessageBuffer& reader) {
    uint16_t event_id{};
    uint64_t sim_tick{};
    uint8_t unknown_u8{};
    (void) reader.read(event_id);
    (void) reader.read(sim_tick);
    (void) reader.read(unknown_u8);

    if(reader.has_read_error()) {
        return nullptr;
    }

    // if(event_id != 0x0) {
    //     return nullptr;
    // }

    auto instance = std::make_unique<SNetUpgradeTowerEvent>(event_id, sim_tick, unknown_u8);
    (void) reader.read(instance->sim_tick);
    (void) reader.read(instance->path);
    (void) reader.read(instance->to_level);
    (void) reader.read(instance->cost);
    (void) reader.read(instance->cost_xp);
    (void) reader.read(instance->net_id);
    (void) reader.read(instance->net_id_version);
    (void) reader.read(instance->tower_type);
    (void) reader.read(instance->eco_receipt_id);


    if(reader.has_read_error()) {
        return nullptr;
    }

    return instance;
}
            

bool SNetUpgradeTowerEvent::encode(NetworkMessageBuffer& writer) const {
    (void) writer.write(this->event_id);
    (void) writer.write(this->sim_tick);
    (void) writer.write(this->unknown_u8);

    (void) writer.write(this->sim_tick);
    (void) writer.write(this->path);
    (void) writer.write(this->to_level);
    (void) writer.write(this->cost);
    (void) writer.write(this->cost_xp);
    (void) writer.write(this->net_id);
    (void) writer.write(this->net_id_version);
    (void) writer.write(this->tower_type);
    (void) writer.write(this->eco_receipt_id);


    return !writer.has_write_error();
}
            
std::map<std::string, std::unique_ptr<GameEvent>(*)(const NetworkMessageBuffer&)> game_event_decoders{
    { "NetworkEvent::SNetLobbyLoadedEvent", SNetLobbyLoadedEvent::decode },
    { "NetworkEvent::SNetDebug_SimulationBundleUploadEvent", SNetDebug_SimulationBundleUploadEvent::decode },
    { "NetworkEvent::SNetRequestMapSkipEvent", SNetRequestMapSkipEvent::decode },
    { "NetworkEvent::SNetDebug_LoadTestBytesEvent", SNetDebug_LoadTestBytesEvent::decode },
    { "NetworkEvent::SNetShowEmoteEvent", SNetShowEmoteEvent::decode },
    { "NetworkEvent::SNetRequestFirePlayerAbilityEvent", SNetRequestFirePlayerAbilityEvent::decode },
    { "NetworkEvent::SNetSendFirstBloodEvent", SNetSendFirstBloodEvent::decode },
    { "NetworkEvent::SNetQueueBloonSetEvent", SNetQueueBloonSetEvent::decode },
    { "NetworkEvent::SNetSendCriticalLifeLostEvent", SNetSendCriticalLifeLostEvent::decode },
    { "NetworkEvent::SNetRequestWithdrawCashFromTowerEvent", SNetRequestWithdrawCashFromTowerEvent::decode },
    { "NetworkEvent::SNetPlaceTowerEvent", SNetPlaceTowerEvent::decode },
    { "NetworkEvent::SNetGameResultEvent", SNetGameResultEvent::decode },
    { "NetworkEvent::SNetGameLoadedEvent", SNetGameLoadedEvent::decode },
    { "NetworkEvent::SNetTriggerMapSkipEvent", SNetTriggerMapSkipEvent::decode },
    { "NetworkEvent::SNetPlayerShieldActivatedEvent", SNetPlayerShieldActivatedEvent::decode },
    { "NetworkEvent::SNetBlingLoadoutEvent", SNetBlingLoadoutEvent::decode },
    { "NetworkEvent::SNetRequestSendTauntEvent", SNetRequestSendTauntEvent::decode },
    { "NetworkEvent::SNetTickEvent", SNetTickEvent::decode },
    { "NetworkEvent::SNetScheduleRoundStartEvent", SNetScheduleRoundStartEvent::decode },
    { "NetworkEvent::SNetSetBadgesEvent", SNetSetBadgesEvent::decode },
    { "NetworkEvent::SNetInitPlayerResponseEvent", SNetInitPlayerResponseEvent::decode },
    { "NetworkEvent::SNetSetBadgesEvent::Badge", SNetSetBadgesEvent__Badge::decode },
    { "NetworkEvent::SNetPatrolPointsSelectedEvent", SNetPatrolPointsSelectedEvent::decode },
    { "NetworkEvent::SNetChangeTargetModeTowerEvent", SNetChangeTargetModeTowerEvent::decode },
    { "NetworkEvent::SNetRequestSwitchFlyingDirectionEvent", SNetRequestSwitchFlyingDirectionEvent::decode },
    { "NetworkEvent::SNetStartGameEvent", SNetStartGameEvent::decode },
    { "NetworkEvent::SNetTowerLoadoutEvent", SNetTowerLoadoutEvent::decode },
    { "NetworkEvent::SNetSendSurrenderNetEvent", SNetSendSurrenderNetEvent::decode },
    { "NetworkEvent::SNetInitPlayerPostponedEvent", SNetInitPlayerPostponedEvent::decode },
    { "NetworkEvent::SNetLastRoundGameEndRequestEvent", SNetLastRoundGameEndRequestEvent::decode },
    { "NetworkEvent::SNetRequestFireWeaponEvent", SNetRequestFireWeaponEvent::decode },
    { "NetworkEvent::SNetMoveToTowerSelectEvent", SNetMoveToTowerSelectEvent::decode },
    { "NetworkEvent::SNetDidFirePlayerAbilityEvent", SNetDidFirePlayerAbilityEvent::decode },
    { "NetworkEvent::SNetTargetTouchPointEvent", SNetTargetTouchPointEvent::decode },
    { "NetworkEvent::SNetAssetBlobUploadEvent", SNetAssetBlobUploadEvent::decode },
    { "NetworkEvent::SNetBombBlitzActivatedEvent", SNetBombBlitzActivatedEvent::decode },
    { "NetworkEvent::SNetCenterPointSelectedEvent", SNetCenterPointSelectedEvent::decode },
    { "NetworkEvent::SNetCollectablesCollectedEvent", SNetCollectablesCollectedEvent::decode },
    { "NetworkEvent::SNetConfirmRoundStartEvent", SNetConfirmRoundStartEvent::decode },
    { "NetworkEvent::SNetDebug_LoadScenarioEvent", SNetDebug_LoadScenarioEvent::decode },
    { "NetworkEvent::SNetDebug_SetSimSpeedEvent", SNetDebug_SetSimSpeedEvent::decode },
    { "NetworkEvent::SNetDebug_StartRoundEvent", SNetDebug_StartRoundEvent::decode },
    { "NetworkEvent::SNetInitPlayerRequestEvent", SNetInitPlayerRequestEvent::decode },
    { "NetworkEvent::SNetDidQueueBloonSetEvent", SNetDidQueueBloonSetEvent::decode },
    { "NetworkEvent::SNetEcoUpdatedEvent", SNetEcoUpdatedEvent::decode },
    { "NetworkEvent::SNetFirePlayerAbilityEvent", SNetFirePlayerAbilityEvent::decode },
    { "NetworkEvent::SNetInitPlayerCompleteEvent", SNetInitPlayerCompleteEvent::decode },
    { "NetworkEvent::SNetLobbyErrorEvent", SNetLobbyErrorEvent::decode },
    { "NetworkEvent::SNetLockInPlacePointSelectedEvent", SNetLockInPlacePointSelectedEvent::decode },
    { "NetworkEvent::SNetReadyToPlayEvent", SNetReadyToPlayEvent::decode },
    { "NetworkEvent::SNetReadyUpEvent", SNetReadyUpEvent::decode },
    { "NetworkEvent::SNetRefereeSelectedTowerLoadoutEvent", SNetRefereeSelectedTowerLoadoutEvent::decode },
    { "NetworkEvent::SNetRemoveBlockerEvent", SNetRemoveBlockerEvent::decode },
    { "NetworkEvent::SNetRemoveTowerEvent", SNetRemoveTowerEvent::decode },
    { "NetworkEvent::SNetRequestCurrentHealthEvent", SNetRequestCurrentHealthEvent::decode },
    { "NetworkEvent::SNetRequestQueueBloonSetEvent", SNetRequestQueueBloonSetEvent::decode },
    { "NetworkEvent::SNetRequestSetTowerTargetPointEvent", SNetRequestSetTowerTargetPointEvent::decode },
    { "NetworkEvent::SNetRequestSimSnapshotEvent", SNetRequestSimSnapshotEvent::decode },
    { "NetworkEvent::SNetRequestStartTowerFlightPathRejoinEvent", SNetRequestStartTowerFlightPathRejoinEvent::decode },
    { "NetworkEvent::SNetRequestSwitchHandEvent", SNetRequestSwitchHandEvent::decode },
    { "NetworkEvent::SNetSendLifeChangedEvent", SNetSendLifeChangedEvent::decode },
    { "NetworkEvent::SNetSetGameMapEvent", SNetSetGameMapEvent::decode },
    { "NetworkEvent::SNetSetNameEvent", SNetSetNameEvent::decode },
    { "NetworkEvent::SNetSimSnapshotEvent", SNetSimSnapshotEvent::decode },
    { "NetworkEvent::SNetStartLoadingEvent", SNetStartLoadingEvent::decode },
    { "NetworkEvent::SNetStartLobbyEvent", SNetStartLobbyEvent::decode },
    { "NetworkEvent::SNetUpgradeTowerEvent", SNetUpgradeTowerEvent::decode },
};
