#pragma once

#include <rltk.hpp>
#include <thread>
#include <memory>

class world_gen {
public:
	void init();
	void destroy();
	void tick(const double duration_ms);

	bool done = false;
private:
	std::unique_ptr<std::thread> world_thread;
};
