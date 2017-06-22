#include "sun_moon.hpp"
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#include "../../main/game_calendar.hpp"
#include "gl_utils.hpp"
#include "populate_gbuffer.hpp"
#include "../../main/game_camera.hpp"
#include "../../systems/input/mouse_input_system.hpp"

namespace map_render {

    glm::vec3 ambient_color;
    glm::vec3 sun_position;
    glm::vec3 sun_color;

    void place_sun_moon() {
        bool moon_mode = false;

        const float time = calendar->hour > 5 && calendar->hour < 18 ? (float)calendar->hour + ((float)calendar->minute/60.0f) : 12.0f;
        //const float time = calendar->hour > 5 && calendar->hour < 18 ? (float)calendar->hour : 5.0f;
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
        rltk::color_t dark_moon{34,63,89};
        const rltk::color_t dawn_light{143,164,191};
        const rltk::color_t noon_sun{201, 226, 255};

        if (moon_mode) {
            float lerp_percent;
            if (calendar->hour < 6) {
                lerp_percent = ((float)calendar->hour + ((float)calendar->minute/60.0f)/100.0f) / 6.0f;
            } else {
                lerp_percent = (24.0f - ((float)calendar->hour) + ((float)calendar->minute/60.0f)/100.0f) / 6.0f;
            }

            int moon_phase = calendar->days_elapsed % 56;
            if (moon_phase > 28) moon_phase = 56 - moon_phase;
            dark_moon.r += moon_phase/2;
            dark_moon.g += moon_phase/2;
            dark_moon.b += moon_phase/2;

            auto ambient_rltk = rltk::lerp(dark_moon, dawn_light, lerp_percent);
            ambient_color = glm::vec3{ (float)ambient_rltk.r / 255.0f, (float)ambient_rltk.g / 255.0f, (float)ambient_rltk.b / 255.0f };
            sun_color = glm::vec3{ 0.7f, 0.7f, 0.8f };
        } else {
            float lerp_percent;
            if (calendar->hour < 12) {
                lerp_percent = ((float)calendar->hour-6.0f + ((float)calendar->minute/60.0f)/100.0f) / 6.0f;
            } else {
                lerp_percent = (24.0f - ((float)calendar->hour+ ((float)calendar->minute/60.0f)/100.0f)) / 6.0f;
            }
            auto ambient_rltk = rltk::lerp(dawn_light, noon_sun, lerp_percent);
            ambient_color = glm::vec3{ (float)ambient_rltk.r / 255.0f, (float)ambient_rltk.g / 255.0f, (float)ambient_rltk.b / 255.0f };
            sun_color = glm::vec3{ 1.0f, 1.0f, 0.97f };
        }
        sun_position = glm::vec3{ sun_x, sun_y, sun_z };
    }
}