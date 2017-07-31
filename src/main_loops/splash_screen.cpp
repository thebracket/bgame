#include "splash_screen.hpp"
#include "../global_assets/texture_storage.hpp"
#include "../bengine/textures.hpp"
#include "../global_assets/shader_storage.hpp"
#include "../bengine/shaders.hpp"
#include "../bengine/simple_sprite.hpp"
#include <iostream>

using namespace bengine;
using namespace assets;

namespace splash_screen {
    double run_time = 0.0;
    float scale = 0.0f;
    float angle = 6.28319f / 2.0f;
    constexpr float angle_step = 0.0174533f * 5.0f;
    float darken = 0.0f;

    /* Loads enough to get things started. */
    void init() {
        bracket_logo = std::make_unique<texture_t>("game_assets/bracket-logo.jpg");
        spriteshader = load_shaders("game_assets/spriteshader_vertex.glsl", "game_assets/spriteshader_fragment.glsl");
        init_simple_sprite();
    }

    void tick(const double &duration_ms) {
        run_time += duration_ms;
        if (run_time > 0.1 && run_time < 500.0f) {
            scale = std::min(0.5f, scale + 0.01f);
            angle -= angle_step;
            if (angle < 0) angle = 0.0f;
            darken += 0.01f;
            if (darken > 1.0f) darken = 1.0f;
        } else {
            darken -= 0.01f;
            if (darken < 0.0f) darken = 0.0f;
        }

        display_sprite(bracket_logo->texture_id, scale, scale, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, angle, darken);
    }
}