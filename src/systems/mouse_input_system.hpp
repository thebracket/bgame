#pragma once

#include <rltk.hpp>

namespace mouse {
	extern int x, y;
	extern int font1_w, font1_h;
	extern int font3_w, font3_h;
	extern int term1x;
	extern int term1y;
	extern int term3x, term3y;
	extern bool clicked;
}

class mouse_input_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
};
