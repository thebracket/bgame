#include "river_builder.hpp"
#include "../planet_builder.hpp"
#include <map>
#include <set>

void planet_rivers(planet_t &planet, bengine::random_number_generator &rng) noexcept {
	set_worldgen_status("Running rivers and lakes");
	const int n_rivers = WORLD_WIDTH/2;
	std::set<int> used_starts;

	for (int i=0; i<n_rivers; ++i) {
		river_t river;

		bool start_ok = false;
		while (!start_ok) {
			river.start_x = rng.roll_dice(1, WORLD_WIDTH)-1;
			river.start_y = rng.roll_dice(1, WORLD_HEIGHT)-1;
			const int pidx = planet.idx(river.start_x, river.start_y);
			if ((planet.landblocks[pidx].type == block_type::MOUNTAINS || planet.landblocks[pidx].type == block_type::HILLS) && used_starts.find(pidx) == used_starts.end()) start_ok = true;
		}
		used_starts.insert(planet.idx(river.start_x, river.start_y));

		std::set<int> used_steps;
		bool done = false;
		int x = river.start_x;
		int y = river.start_y;
		while (!done) {
			std::map<uint8_t, std::pair<int,int>> candidates;
			if (x > 0 && used_steps.find(planet.idx(x-1, y))==used_steps.end()) candidates.insert({planet.landblocks[planet.idx(x-1, y)].height, {x-1, y}});
			if (x < WORLD_WIDTH-1 && used_steps.find(planet.idx(x+1, y))==used_steps.end()) candidates.insert({planet.landblocks[planet.idx(x+1, y)].height, {x+1, y}});
			if (y > 0 && used_steps.find(planet.idx(x, y-1))==used_steps.end()) candidates.insert({planet.landblocks[planet.idx(x, y-1)].height, {x, y-1}});
			if (y < WORLD_HEIGHT-1 && used_steps.find(planet.idx(x, y+1))==used_steps.end()) candidates.insert({planet.landblocks[planet.idx(x, y+1)].height, {x, y+1}});
			river_step_t step;
			if (candidates.empty()) {
				done = true;
			} else {

				for (const auto &test : planet.rivers) {
					if (!done) {
						for (const auto &step : test.steps) {
							if (x==step.x && y==step.y) { done=true; break; }
						}
					}
				}

				if (!done) {
					step.x = candidates.begin()->second.first;
					step.y = candidates.begin()->second.second;

					if (planet.landblocks[planet.idx(x,y)].type == block_type::WATER || x == 0 || x == WORLD_WIDTH || y ==0 || y==WORLD_HEIGHT) {
						done = true;
					} else {
						river.steps.push_back(step);
						used_steps.insert(planet.idx(step.x, step.y));
						x = step.x;
						y = step.y;
					}
				}
			}
		}

		planet_display_update_zoomed(planet, WORLD_WIDTH/2, WORLD_HEIGHT/2);
		planet.rivers.push_back(river);
	}
}
