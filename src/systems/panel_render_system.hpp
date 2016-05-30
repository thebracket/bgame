#pragma once

#include <rltk.hpp>
#include <vector>
#include "../game_globals.hpp"
#include "boost/optional.hpp"

class panel_render_system : public rltk::base_system {
public:
	panel_render_system() { }
	virtual void update(const double duration_ms) override;
	virtual void configure() override;
private:
	void render_play_mode();
	void render_design_mode();
};