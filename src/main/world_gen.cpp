#include "world_gen.hpp"
#include "../planet/planet_builder.hpp"
#include "game_globals.hpp"
#include "../utils/telemetry.hpp"
#include "menu_helper.hpp"
#include "../systems/gui/imgui_helper.hpp"
#include "../external/imgui-sfml/imgui-SFML.h"
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <OpenGL/glu.h>
#include <iostream>

constexpr int WORLD_LAYER=1;
constexpr int LOG_LAYER=2;
constexpr int BACKDROP_LAYER=2;

using namespace rltk;
using namespace rltk::colors;

void world_gen::init() {
	auto window_size = get_window()->getSize();
	const int window_width = window_size.x;
	const int window_height = window_size.y;

    gui->add_owner_layer(BACKDROP_LAYER, 0, 0, window_width, window_height, resize_fullscreen, draw_splash_backdrop);
	gui->add_layer(WORLD_LAYER, 0, 0, window_width, window_height, game_config.game_font, resize_fullscreen, true);
	done = false;
	setup_build_planet();
	call_home("worldgen");
	seed = rng.roll_dice(1, std::numeric_limits<int>::max());
    optional_display_hook = rltk::optional_display_hook;
}

void world_gen::destroy() {
	gui->delete_layer(WORLD_LAYER);
	world_thread->join();
	world_thread.reset();
	planet_builder_display.reset();
    call_home("worldgen_done");
}

void world_gen::start_thread() {
	world_thread = std::make_unique<std::thread>(build_planet, seed, water, plains, starting_settlers, strict_beamdown);
}

void render_hook();

void world_gen::render_menu() {
	ImGui::Begin("World Generation");

	ImGui::InputInt("World Seed", &seed);
	ImGui::SliderInt("Water Level", &water, 1, 4);
	ImGui::SliderInt("Plains Level", &plains, 1, 4);
	ImGui::SliderInt("Starting Settlers", &starting_settlers, 1, 20);
	ImGui::Checkbox("Require Teleport Beacon for Beamdown", &strict_beamdown);
	if (ImGui::Button("Create World")) {
		mode = WG_RUNNING;
		start_thread();
        gui->delete_layer(BACKDROP_LAYER);
        rltk::optional_display_hook = render_hook;
	}
	ImGui::End();
}

float wg_rotate_angle = 0.0f;

void render_hook() {
    planet_builder_lock.lock();

    // Push GL
    get_window()->pushGLStates();
    get_window()->resetGLStates();

    // Draw it
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glShadeModel(GL_SMOOTH);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.f, 1.f, 1.f, 300.0f);//fov, aspect, zNear, zFar

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(wg_rotate_angle, 0.0f, 1.0f, 0.0f);
    //glTranslatef(0.f, -160.f, -150.f);
    glTranslatef(0.f, -60.f, -50.f);
    glScalef(2.0f, 0.3f, 2.0f);
    wg_rotate_angle += 1.0f;
    if (wg_rotate_angle > 360.0f) wg_rotate_angle = 0.0f;

    glEnable(GL_TEXTURE_2D);
    sf::Texture::bind(rltk::get_texture(term(WORLD_LAYER)->get_font_tag()));

    glBegin(GL_QUADS);
    for (int y=0; y<WORLD_HEIGHT-1; ++y) {
        for (int x=0; x<WORLD_WIDTH-1; ++x) {
            const int idx = planet.idx(x,y);
            const float altitude = (*planet_builder_display.get())[idx].altitude;

            float red = (float)(*planet_builder_display.get())[idx].foreground.r / 255.0f;
            float green = (float)(*planet_builder_display.get())[idx].foreground.g / 255.0f;
            float blue = (float)(*planet_builder_display.get())[idx].foreground.b / 255.0f;

            float slope = 1.0f - std::abs(altitude - (*planet_builder_display.get())[planet.idx(x+1, y)].altitude) / 6.0f;
            //std::cout << slope << "\n";
            red *= slope;
            green *= slope;
            blue *= slope;

            float X = ((float)x - WORLD_WIDTH/2.0f);
            float Y = ((float)y - WORLD_HEIGHT/2.0f);

            // Scope for rendering the world tile
            {
                const int tex_x = ((*planet_builder_display.get())[idx].terrain_glyph % 16) * 24;
                const int tex_y = ((*planet_builder_display.get())[idx].terrain_glyph / 16) * 24;
                const float tex_xf = (float) tex_x / 384.0f;
                const float tex_yf = (float) tex_y / 768.0f;
                const float tex_xsize = 24.0f / 384.0f;
                const float tex_ysize = 24.0f / 768.0f;

                glColor3f(red, green, blue);

                glTexCoord2f(tex_xf, tex_yf);
                glVertex3f(X, altitude, Y);
                glTexCoord2f(tex_xf, tex_yf + tex_ysize);
                glVertex3f(X, (*planet_builder_display.get())[planet.idx(x, y + 1)].altitude, Y + 1.0f);
                glTexCoord2f(tex_xf + tex_xsize, tex_yf + tex_ysize);
                glVertex3f(X + 1.0f, (*planet_builder_display.get())[planet.idx(x + 1, y + 1)].altitude, Y + 1.0f);
                glTexCoord2f(tex_xf + tex_xsize, tex_yf);
                glVertex3f(X + 1.0f, (*planet_builder_display.get())[planet.idx(x + 1, y)].altitude, Y);
            }

            if ((*planet_builder_display.get())[planet.idx(x,y+1)].unit_glyph > 0) {
                const int tex_x = ((*planet_builder_display.get())[idx].unit_glyph % 16) * 24;
                const int tex_y = ((*planet_builder_display.get())[idx].unit_glyph / 16) * 24;
                const float tex_xf = (float) tex_x / 384.0f;
                const float tex_yf = (float) tex_y / 768.0f;
                const float tex_xsize = 24.0f / 384.0f;
                const float tex_ysize = 24.0f / 768.0f;

                glColor3f(1.0f, 1.0f, 1.0f);

                glTexCoord2f(tex_xf, tex_yf);
                glVertex3f(X, altitude, Y);
                glTexCoord2f(tex_xf, tex_yf + tex_ysize);
                glVertex3f(X, (*planet_builder_display.get())[planet.idx(x, y + 1)].altitude, Y + 1.0f);
                glTexCoord2f(tex_xf + tex_xsize, tex_yf + tex_ysize);
                glVertex3f(X + 1.0f, (*planet_builder_display.get())[planet.idx(x + 1, y + 1)].altitude, Y + 1.0f);
                glTexCoord2f(tex_xf + tex_xsize, tex_yf);
                glVertex3f(X + 1.0f, (*planet_builder_display.get())[planet.idx(x + 1, y)].altitude, Y);
            }
        }
    }

    glEnd();

    // Pop GL
    get_window()->popGLStates();
    planet_builder_lock.unlock();

    // Call ImGui
    ImGui::Render();
}

void world_gen::tick(const double duration_ms) {
	term(WORLD_LAYER)->clear();

	if (mode == WG_MENU) {
		render_menu();
	} else {
        planet_builder_lock.lock();
        term(WORLD_LAYER)->print_center(2, planet_builder_status);
        planet_builder_lock.unlock();

		if (is_planet_build_complete()) {
            done = true;
            rltk::optional_display_hook = optional_display_hook;
        }
	}
}
