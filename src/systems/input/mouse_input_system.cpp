#include "mouse_input_system.hpp"
#include "../../utils/gl/map_render.hpp"
#include "../../planet/constants.hpp"

namespace mouse {
	int x, y;
	int font1_w, font1_h;
	int font3_w, font3_h;
    int font4_w, font4_h;
	int term1x;
	int term1y;
	int term3x,term3y;
	int term4x,term4y;
	bool clicked;

	int mouse_world_x=0;
	int mouse_world_y=0;
	int mouse_world_z=0;
}

void mouse_input_system::configure() {
    system_name = "Mouse Input System";
}

void mouse_input_system::update(const double ms) {
	std::tie(mouse::x, mouse::y) = rltk::get_mouse_position();
	std::tie(mouse::font1_w, mouse::font1_h) = rltk::term(1)->get_font_size();
	std::tie(mouse::font3_w, mouse::font3_h) = rltk::term(3)->get_font_size();
    std::tie(mouse::font4_w, mouse::font4_h) = rltk::term(4)->get_font_size();
	mouse::term1x = mouse::x / mouse::font1_w;
	mouse::term1y = mouse::y / mouse::font1_h;
	mouse::term3x = mouse::x / mouse::font3_w;
	mouse::term3y = mouse::y / mouse::font3_h;
    mouse::term4x = mouse::x / mouse::font4_w;
    mouse::term4y = mouse::y / mouse::font4_h;
	mouse::clicked = false;
	mouse_damper += ms;
	if (rltk::get_mouse_button_state(rltk::button::LEFT) && mouse_damper > 500.0) {
		mouse::clicked = true;
		mouse_damper = 0;
	}

	// Read back from the texture to find out where we really are in GL-land!
	auto mouse_pos = map_render::readback_texture_pixel(mouse::x, mouse::y);
	std::tie(mouse::mouse_world_x, mouse::mouse_world_y, mouse::mouse_world_z) = mouse_pos;
	if (mouse::mouse_world_x >= REGION_WIDTH) mouse::mouse_world_x = 1;
	if (mouse::mouse_world_y >= REGION_HEIGHT) mouse::mouse_world_y = 1;
	if (mouse::mouse_world_z >= REGION_DEPTH) mouse::mouse_world_z = 1;
}
