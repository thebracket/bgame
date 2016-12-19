#pragma once

#include <rltk.hpp>

class mode_play_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
    int last_mouse_x = 0;
	int last_mouse_y = 0;
	float mouse_dwell_time = 0.0F;

	int menu_x,menu_y;

	void show_tooltip(const int world_x, const int world_y, const int tile_idx);
	void show_tilemenu();
};