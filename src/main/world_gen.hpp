#pragma once

#include <rltk.hpp>
#include <thread>
#include <memory>

enum world_gen_mode_t { WG_MENU, WG_RUNNING };

class world_gen {
public:
	void init();
	void destroy();
	void tick(const double duration_ms);

	bool done = false;
private:
    world_gen_mode_t mode = WG_MENU;
	std::unique_ptr<std::thread> world_thread;
    void render_menu();
    void start_thread();
    int seed;
    int plains = 3;
    int water = 3;
    int starting_settlers = 10;
    bool strict_beamdown = false;
};
