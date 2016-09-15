#include "world_system.hpp"
#include "../messages/messages.hpp"
#include "../main/game_globals.hpp"
#include "../planet/builder/sentient_builder.hpp"

void world_system::configure() {
    system_name = "Background World";

    subscribe_mbox<day_elapsed_message>();
}

inline void world_evaluate_move_option(std::vector<int> &candidates, planet_t &planet, const int &idx) {
    if (planet.landblocks[idx].type == block_type::WATER) return;

    int weight = 1;
    if (planet.idx(current_region->region_x, current_region->region_y) == idx) {
        // Heavily favor visiting the player
        weight = 8;
    } else {
        if (planet.landblocks[idx].type == block_type::PLAINS) weight=4;
        if (planet.landblocks[idx].type == block_type::HILLS) weight=2;
        if (planet.landblocks[idx].type == block_type::MOUNTAINS) weight=1;
        if (planet.landblocks[idx].type == block_type::MARSH) weight=2;
        if (planet.landblocks[idx].type == block_type::PLATEAU) weight=4;
        if (planet.landblocks[idx].type == block_type::HIGHLANDS) weight=3;
        if (planet.landblocks[idx].type == block_type::COASTAL) weight=4;
        if (planet.landblocks[idx].type == block_type::SALT_MARSH) weight=1;
    }

    for (int i=0; i<weight; ++i) candidates.push_back(idx);
}

void world_system::update(const double ms) {
    std::queue<day_elapsed_message> * day = mbox<day_elapsed_message>();
	while (!day->empty()) {
        day_elapsed_message msg = day->front();
        day->pop();

        std::cout << "A new day has dawned\n";
        std::size_t peep_id = 0;
        for (auto &peep : planet.civs.unimportant_people) {
            if (!peep.deceased && !(peep.world_x == current_region->region_x && peep.world_y == current_region->region_y)) {
                // Move randomly
                std::vector<int> candidates;
                if (peep.world_y > 2) world_evaluate_move_option(candidates, planet, planet.idx(peep.world_x, peep.world_y-1));
                if (peep.world_y < WORLD_HEIGHT-2) world_evaluate_move_option(candidates, planet, planet.idx(peep.world_x, peep.world_y+1));
                if (peep.world_x > 2) world_evaluate_move_option(candidates, planet, planet.idx(peep.world_x-1, peep.world_y));
                if (peep.world_x < WORLD_WIDTH-2) world_evaluate_move_option(candidates, planet, planet.idx(peep.world_x+1, peep.world_y));

                if (!candidates.empty()) {
                    const int roll = rng.roll_dice(1, candidates.size())-1;
                    const int destination = candidates[roll];
                    //std::cout << peep_id << " moved\n";
                    const int new_x = destination % WORLD_WIDTH;
                    const int new_y = destination / WORLD_WIDTH;
                    const int old_x = peep.world_x;
                    const int old_y = peep.world_y;
                    peep.world_x = new_x;
                    peep.world_y = new_y;

                    if (peep.world_x == current_region->region_x && peep.world_y == current_region->region_y) {
                        int edge = 0;
                        if (old_x > new_x) edge = 3;
                        if (old_x < new_x) edge = 2;
                        if (old_y > new_y) edge = 1;
                        if (old_y < new_y) edge = 0;

                        int x,y,z;
                        if (edge == 0) {
                            y = 2;
                            x = REGION_WIDTH/2;
                            z = get_ground_z(*current_region, x, y);
                        } else if (edge == 1) {
                            y = REGION_HEIGHT-2;
                            x = REGION_WIDTH/2;
                            z = get_ground_z(*current_region, x, y);
                        } else if (edge == 2) {
                            x = 2;
                            y = REGION_HEIGHT/2;
                            z = get_ground_z(*current_region, x, y);
                        } else {
                            x = REGION_WIDTH-2;
                            y = REGION_HEIGHT/2;
                            z = get_ground_z(*current_region, x, y);
                        }

                        while (!current_region->tile_flags[mapidx(x,y,z)].test(CAN_STAND_HERE)) {
                            if (edge == 0 || edge == 1) {
                                ++x;
                            } else {
                                ++y;
                            }
                            z = get_ground_z(*current_region, x, y);
                        }

                        std::cout << "Spawning new arrivals (" << peep_id << ") on edge " << edge << "\n";
                        for (int i=0; i<5; ++i)
                            create_sentient(x,y,z,rng,planet,*current_region,peep_id);
                    }
                }

            }
            ++peep_id;
        }
    }
}
