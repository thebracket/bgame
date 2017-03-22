#pragma once

#include <rltk.hpp>

namespace mouse {
	extern int x, y;
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
