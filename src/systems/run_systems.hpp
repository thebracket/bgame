#pragma once

namespace systems {
    constexpr int CAMERA_SYSTEM = 1;
    constexpr int HUD_SYSTEM = 2;

    void init();
    void run(const double &duration_ms);
}