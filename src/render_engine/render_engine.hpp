#pragma once

#include <memory>
#include "fbo/gbuffer.hpp"

namespace render {
    extern bool camera_moved;
    extern bool models_changed;
    void render_gl(const double &duration_ms);
	extern std::unique_ptr<gbuffer_t> gbuffer;
}