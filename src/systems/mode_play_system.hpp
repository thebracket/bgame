#pragma once

#include <rltk.hpp>

class mode_play_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
    int last_mouse_x = 0;
	int last_mouse_y = 0;
	double mouse_dwell_time = 0.0;
};