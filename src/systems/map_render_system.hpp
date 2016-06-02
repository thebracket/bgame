#pragma once

#include <rltk.hpp>
#include "../game_globals.hpp"

class map_render_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
	bool dirty = true;
	bool renderables_changed = true;
	void update_clipping_rectangle();
};

