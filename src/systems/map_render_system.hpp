#pragma once

#include <rltk.hpp>
#include <vector>
#include "../game_globals.hpp"
#include "boost/optional.hpp"

extern std::vector<boost::optional<rltk::vchar>> renderables;

class map_render_system : public rltk::base_system {
public:
	map_render_system() { renderables.resize(REGION_WIDTH * REGION_HEIGHT * REGION_DEPTH); }
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
	bool dirty = true;
	bool renderables_changed = true;
	void update_clipping_rectangle();
};

