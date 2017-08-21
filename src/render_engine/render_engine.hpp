#pragma once

namespace render {
    extern bool camera_moved;
    extern bool models_changed;
    void render_gl(const double &duration_ms);
}