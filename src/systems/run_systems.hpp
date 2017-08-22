#pragma once

namespace systems {
    constexpr int CAMERA_SYSTEM = 1;
    constexpr int HUD_SYSTEM = 2;
    constexpr int TICK_SYSTEM = 3;
    constexpr int CALENDAR_SYSTEM = 4;

    void init();
    void run(const double &duration_ms);
}