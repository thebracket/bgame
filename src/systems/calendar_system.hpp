#pragma once

#include <rltk.hpp>

const double MS_PER_TICK = 5.0;

class calendar_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override;
private:
	double time_count = 100.0;
};
