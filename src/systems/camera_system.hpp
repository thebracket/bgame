#pragma once

#include <rltk.hpp>
#include "../game_globals.hpp"
#include <vector>

extern std::vector<int> render_tiles;

class camera_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
	bool dirty = true;
	void update_clipping_rectangle();
	void build_render_tiles();
	int deep_dive(const int &x, const int &y, const int &z);
	void build_render_tiles_with_vertical_dive();
	void build_render_tiles_without_vertical_dive();
};
