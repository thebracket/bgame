#pragma once

#include <rltk.hpp>

class world_gen {
public:
	void init();
	void destroy();
	void tick(const double duration_ms);
};
