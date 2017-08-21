#pragma once

namespace systems {
    constexpr int CAMERA_SYSTEM = 1;

    void init();
    void run(const double &duration_ms);
}