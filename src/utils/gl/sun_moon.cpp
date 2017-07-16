#include "sun_moon.hpp"
#include "gl_include.hpp"
#include "../../main/game_calendar.hpp"
#include "gl_utils.hpp"
#include "populate_gbuffer.hpp"
#include "../../main/game_camera.hpp"
#include "../../systems/input/mouse_input_system.hpp"

namespace map_render {

    glm::vec3 ambient_color;
    glm::vec3 sun_position;
    glm::vec3 sun_color;
    glm::vec3 camera_position_v; // This needs to be used in the shader for camera position, rather than the raw pos to provide real reflectivity


    inline float calc_sun_time() {
        return (float)calendar->hour + ((float)calendar->minute/60.0f);
    }

    void place_sun_moon() {
        bool moon_mode = false;

        const float time = calc_sun_time();
        const float time_as_fraction =  1.0f - (time / 24.0f); // Inverted because the sun goes west to east
        const float time_as_degrees = (time_as_fraction * 360.0f) + 270.0f; // Add to put midnight underneath the world
        const float radians = time_as_degrees * 3.14159265f/180.0f;
        constexpr float SUN_DISTANCE = 256.0f;

        float sun_x = (SUN_DISTANCE * std::cos(radians)) + 128.0f;
        float sun_y = (SUN_DISTANCE * std::sin(radians)) + 128.0f;
        float sun_z = 129.0f;

        if (calendar->hour < 6 || calendar->hour > 17) {
            moon_mode = true;
        }

        // Setup ambient light
        rltk::color_t dark_moon{100, 100, 105};
        const rltk::color_t dawn_light{100, 100, 105};
        const rltk::color_t noon_sun{100, 100, 105};

        if (moon_mode) {
            float lerp_percent;
            if (calendar->hour < 6) {
                lerp_percent = ((float)calendar->hour + ((float)calendar->minute/60.0f)/100.0f) / 6.0f;
            } else {
                lerp_percent = (24.0f - ((float)calendar->hour) + ((float)calendar->minute/60.0f)/100.0f) / 6.0f;
            }

            /*int moon_phase = calendar->days_elapsed % 56;
            if (moon_phase > 28) moon_phase = 56 - moon_phase;
            const int moon_phase_light = moon_phase/3;
            dark_moon.r += moon_phase_light;
            dark_moon.g += moon_phase_light;
            dark_moon.b += moon_phase_light;*/

            auto ambient_rltk = rltk::lerp(dark_moon, dawn_light, lerp_percent);
            ambient_color = glm::vec3{ (float)ambient_rltk.r / 255.0f, (float)ambient_rltk.g / 255.0f, (float)ambient_rltk.b / 255.0f };
            sun_color = glm::vec3{ 1.0f, 1.0f, 1.0f };
        } else {
            float lerp_percent;
            if (calendar->hour < 12) {
                lerp_percent = ((float)calendar->hour-6.0f + ((float)calendar->minute/60.0f)/100.0f) / 6.0f;
            } else {
                lerp_percent = (24.0f - ((float)calendar->hour+ ((float)calendar->minute/60.0f)/100.0f)) / 6.0f;
            }
            auto ambient_rltk = rltk::lerp(dawn_light, noon_sun, lerp_percent);
            ambient_color = glm::vec3{ (float)ambient_rltk.r / 255.0f, (float)ambient_rltk.g / 255.0f, (float)ambient_rltk.b / 255.0f };
            sun_color = glm::vec3{ 1.0f, 1.0f, 1.0f };
        }
        sun_position = glm::vec3{ sun_x, sun_y, sun_z };
    }
}