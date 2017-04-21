#include "mouse_input_system.hpp"
#include "../../utils/gl/map_render.hpp"
#include "../../planet/constants.hpp"
#include "../../messages/messages.hpp"
#include "../../main/game_camera.hpp"

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
    mouse::mouse_world_z = camera_position->region_z;
	if (mouse::mouse_world_x >= REGION_WIDTH) mouse::mouse_world_x = 1;
	if (mouse::mouse_world_y >= REGION_HEIGHT) mouse::mouse_world_y = 1;
	if (mouse::mouse_world_z >= REGION_DEPTH) mouse::mouse_world_z = 1;

    //std::cout << mouse::mouse_world_z << ", camera is " << camera_position->region_z << "\n";
    if (rltk::get_mouse_button_state(button::WHEEL_UP)) {
        set_mouse_button_state(button::WHEEL_UP, false);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
            emit_deferred(camera_move_requested_message{6, 1});
        } else {
            ++camera->zoom_level;
            if (camera->zoom_level > 40) camera->zoom_level = 40;
        }
    } else if (rltk::get_mouse_button_state(button::WHEEL_DOWN)) {
        set_mouse_button_state(button::WHEEL_DOWN, false);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
            emit_deferred(camera_move_requested_message{5, 1});
        } else {
            --camera->zoom_level;
            if (camera->zoom_level < 12) camera->zoom_level = 12;
        }
    }
}
