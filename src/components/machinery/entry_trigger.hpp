#pragma once

enum trigger_type { TRIGGER_CAGE, TRIGGER_STONEFALL, TRIGGER_BLADE, TRIGGER_PRESSURE };

struct entry_trigger_t {
    bool active = true;
    trigger_type type = TRIGGER_CAGE;

	entry_trigger_t() = default;
    explicit entry_trigger_t(const trigger_type &t) noexcept : type(t) {}
    entry_trigger_t(const trigger_type &t, const bool &enabled) noexcept : active(enabled), type(t) {}
};

