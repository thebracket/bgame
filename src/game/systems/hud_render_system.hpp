#pragma once

#include "../../engine/ecs.hpp"

using engine::base_system;

class hud_render_system: public base_system
{
public:
	hud_render_system()
	{
		system_name = "HUD Render System";
	}
	virtual void tick(const double &duration_ms);
private:
	void render_hud_base();
	void render_power_bar();
	void render_date_time();
	void render_paused();
};
