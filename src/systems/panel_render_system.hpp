#pragma once

#include <rltk.hpp>
#include <vector>
#include "../game_globals.hpp"
#include "boost/optional.hpp"

class panel_render_system : public rltk::base_system {
public:
	panel_render_system() { }
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
	void render_header();

	void render_mode_select();

	void render_play_mode();
	void render_design_mode();
	void render_units_mode();
	void render_settler_mode();
	void render_work_mode();

	std::size_t selected_settler;

	double mouse_damper = 100.0;
};