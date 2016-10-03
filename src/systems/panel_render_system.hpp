#pragma once

#include <rltk.hpp>
#include <vector>
#include "../main/game_globals.hpp"
#include "boost/optional.hpp"

class panel_render_system : public rltk::base_system {
public:
	panel_render_system() { }
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
	void render_mode_select(const double duration_ms);

	void render_work_mode();

	double mouse_damper = 100.0;
};