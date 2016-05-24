#pragma once

#include <rltk.hpp>
#include <vector>
#include "../game_globals.hpp"
#include "boost/optional.hpp"

class map_render_system : public rltk::base_system {
public:
	map_render_system() { renderables.resize(REGION_WIDTH * REGION_HEIGHT * REGION_DEPTH); }
	virtual void update(const double duration_ms) override;
private:
	bool dirty = true;
	void update_clipping_rectangle();
	rltk::vchar get_render_char(const int idx) const;
	rltk::vchar get_render_char_for_base(const uint8_t base_type) const;
	std::vector<boost::optional<rltk::vchar>> renderables;
};