#include "mouse_input_system.hpp"

namespace mouse {
	int x, y;
	int font1_w, font1_h;
	int font3_w, font3_h;
	int term1x;
	int term1y;
	int term3x,term3y;
	bool clicked;
}

void mouse_input_system::configure() {
    system_name = "Mouse Input System";
}

void mouse_input_system::update(const double ms) {
	std::tie(mouse::x, mouse::y) = rltk::get_mouse_position();
	std::tie(mouse::font1_w, mouse::font1_h) = rltk::term(1)->get_font_size();
	std::tie(mouse::font3_w, mouse::font3_h) = rltk::term(3)->get_font_size();
	mouse::term1x = mouse::x / mouse::font1_w;
	mouse::term1y = mouse::y / mouse::font1_h;
	mouse::term3x = mouse::x / mouse::font3_w;
	mouse::term3y = mouse::y / mouse::font3_h;
	mouse::clicked = false;
	mouse_damper += ms;
	if (rltk::get_mouse_button_state(rltk::button::LEFT) && mouse_damper > 500.0) {
		mouse::clicked = true;
		mouse_damper = 0;
	}
}
