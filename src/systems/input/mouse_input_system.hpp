#pragma once

#include <rltk.hpp>

namespace mouse {
	extern int x, y;
	extern int font1_w, font1_h;
	extern int font3_w, font3_h;
    extern int font4_w, font4_h;
	extern int term1x;
	extern int term1y;
	extern int term3x, term3y;
	extern int term4x, term4y;
	extern bool clicked;
	extern int mouse_world_x, mouse_world_y, mouse_world_z;
}

class mouse_input_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
	double mouse_damper = 100.0;
};
