#pragma once

namespace bengine {
    void init_simple_sprite();
    void display_sprite(const unsigned int &texture_id,
                        const float &scale_x = 1.0f,
                        const float &scale_y = 1.0f,
                        const float &x = 0.0f,
                        const float &y = 0.0f,
                        const float &z = 0.0f,
                        const float &rotate_x = 0.0f,
                        const float &rotate_y = 0.0f,
                        const float &rotate_z = 0.0f,
                        const float &angle = 0.0f,
                        const float &darken = 1.0f);
}