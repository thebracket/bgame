#pragma once

#include <rltk.hpp>
#include "../main/game_globals.hpp"

class map_render_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
	bool dirty = true;
	bool renderables_changed = true;
};

namespace map_render_sys {
	extern bool mouse_in_terminal;
	extern int mouse_term_x;
	extern int mouse_term_y;
	extern bool building_possible;
}

extern uint32_t glyph_cycle;
