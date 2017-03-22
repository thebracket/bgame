#include "mouse_input_system.hpp"
#include "../../utils/gl/map_render.hpp"
#include "../../planet/constants.hpp"

namespace mouse {
	int x=0;
	int y=0;
	bool clicked = false;
	int mouse_world_x=0;
	int mouse_world_y=0;
	int mouse_world_z=0;
}

void mouse_input_system::configure() {
    system_name = "Mouse Input System";
}

void mouse_input_system::update(const double ms) {
	std::tie(mouse::x, mouse::y) = rltk::get_mouse_position();
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
