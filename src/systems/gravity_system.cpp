#include "gravity_system.hpp"
#include "../messages/tick_message.hpp"
#include "../messages/entity_moved_message.hpp"
#include "../messages/inflict_damage_message.hpp"
#include "../messages/map_dirty_message.hpp"
#include "../components/falling.hpp"
#include "../components/building.hpp"
#include "../main/game_globals.hpp"
#include "../messages/emit_particles_message.hpp"
#include "../raws/raws.hpp"
#include <deque>
#include <array>
#include "../messages/recalculate_mining_message.hpp"
#include "../messages/perform_mining.hpp"

void gravity_system::configure() {
    system_name = "Gravity System";
    subscribe_mbox<tick_message>();
    subscribe_mbox<tile_removed_message>();
}

void gravity_system::update(const double ms) {
    std::queue<tile_removed_message> * removals = mbox<tile_removed_message>();
	while (!removals->empty()) {
        tile_removed_message t = removals->front();
		removals->pop();

        std::vector<bool> connected;
        std::vector<bool> visited;
        connected.resize(REGION_TILES_COUNT);
        visited.resize(REGION_TILES_COUNT);
        std::fill(connected.begin(), connected.end(), false);
        std::fill(visited.begin(), visited.end(), false);
        std::deque<int> targets;
        targets.push_back(mapidx(0, 0, 0));
        while (!targets.empty()) {
            const int idx = targets.front();
            targets.pop_front();

            if (!visited[idx]) {
                visited[idx] = true;

                int x,y,z;
                std::tie(x,y,z) = idxmap(idx);
                if (z == 0) {
                    connected[idx] = true;
                    if (x>0) targets.push_back(mapidx(x-1,y,z));
                    if (x<REGION_WIDTH) targets.push_back(mapidx(x+1,y,z));
                    if (y>0) targets.push_back(mapidx(x,y-1,z));
                    if (y<REGION_WIDTH) targets.push_back(mapidx(x,y+1,z));
                    if (z<REGION_DEPTH-1) targets.push_back(mapidx(x,y,z+1));
                    //std::cout << "Connected " << x << "," << y << "," << z << ", type " << +current_region->tile_type[idx] << "\n";
                } else {
                    if (current_region->tile_type[idx] != tile_type::OPEN_SPACE || current_region->solid[idx] || current_region->tile_flags[idx].test(CAN_STAND_HERE)) {
                        connected[idx] = true;
                        //std::cout << "Connected " << x << "," << y << "," << z << ", type " << +current_region->tile_type[idx] << "\n";
                        if (x>0) targets.push_back(mapidx(x-1,y,z));
                        if (x<REGION_WIDTH) targets.push_back(mapidx(x+1,y,z));
                        if (y>0) targets.push_back(mapidx(x,y-1,z));
                        if (y<REGION_WIDTH) targets.push_back(mapidx(x,y+1,z));
                        if (z<REGION_DEPTH-1) targets.push_back(mapidx(x,y,z+1));
                        if (z>0) targets.push_back(mapidx(x,y,z-1));               
                    }
                }
            }
        }

        for (int idx = 0; idx < REGION_TILES_COUNT; ++idx) {
            if (current_region->tile_type[idx] != tile_type::OPEN_SPACE && !connected[idx]) {
                int x,y,z;
                std::tie(x,y,z) = idxmap(idx);
                emit_deferred(emit_particles_message{1, x, y, z});
                std::cout << "Collapse: " << x << "/" << y << "/" << z << "\n";

                // Constructions de-construct
                if (current_region->tile_flags[idx].test(CONSTRUCTION)) {
                    if (current_region->tile_type[idx] == tile_type::FLOOR || current_region->tile_type[idx] == tile_type::WALL
                        || current_region->tile_type[idx] == tile_type::STAIRS_DOWN || current_region->tile_type[idx] == tile_type::STAIRS_UP
                        || current_region->tile_type[idx] == tile_type::STAIRS_UPDOWN || current_region->tile_type[idx] == tile_type::WINDOW)
                    {
                        spawn_item_on_ground(x, y, z, "block", current_region->tile_material[idx]);
                    } else if (current_region->tile_type[idx] != tile_type::OPEN_SPACE) {
                        emit_deferred(perform_mining_message{idx, 1, x, y, z});
                    }
                }

                // Other tiles become objects and will naturally fall

                // Tile resets and re-calculation
                current_region->tile_type[idx] = tile_type::OPEN_SPACE;
                current_region->tile_flags[idx].reset(CONSTRUCTION);
                current_region->tile_flags[idx].reset(CAN_STAND_HERE);
                for (int Z=z-2; Z<z+2; Z++) {
                    for (int Y=y-2; Y<y+2; Y++) {
                        for (int X=x-1; X<x+2; X++) {
                            current_region->tile_calculate(X,Y,Z);
                        }
                    }
                }
                emit_deferred(recalculate_mining_message{});                
            }
        }
    }

    std::queue<tick_message> * ticks = mbox<tick_message>();
	while (!ticks->empty()) {
		ticks->pop();

        // Entities that are currently falling
        boost::container::flat_set<std::size_t> falling_entities;

        each<position_t, falling_t>([&falling_entities] (entity_t &e, position_t &pos, falling_t &fall) {
            falling_entities.insert(e.id);
            const auto idx = mapidx(pos.x, pos.y, pos.z);

            if (current_region->tile_flags[idx].test(CAN_STAND_HERE)) {
                // We hit the ground
                if (fall.distance > 2) {
                    emit_deferred(inflict_damage_message{e.id, rng.roll_dice(fall.distance, 6), "falling damage"});
                    delete_component<falling_t>(e.id);
                }
            } else {
                // Still falling
                emit_deferred(entity_wants_to_move_message{e.id, position_t{pos.x, pos.y, pos.z-1}});
                ++fall.distance;
            }
        });

        // See if any entities need to fall
        each<position_t>([&falling_entities] (entity_t &e, position_t &pos) {
            if (falling_entities.find(e.id) == falling_entities.end()) { // Ignore entities that are already falling
                const auto idx = mapidx(pos.x, pos.y, pos.z);
                const auto idx_below = mapidx(pos.x, pos.y, pos.z);

                bool is_falling = true;

                // We're standing on a solid tile - don't fall
                if (current_region->tile_flags[idx].test(CAN_STAND_HERE)) is_falling = false;

                // Incomplete buildings shouldn't fall
                if (is_falling) {
                    auto building = e.component<building_t>();
                    if (building && !building->complete) {
                        is_falling = false;
                    } else if (building) {
                        // Deconstruct the building
                        for (auto &component : building->built_with) {
                            spawn_item_on_ground(pos.x, pos.y, pos.z, component.first, component.second);
                        }
                        delete_entity(e.id);
                        is_falling = false;
                    }
                }

                if (is_falling) {
                    e.assign(falling_t{0});
                }
            }
        });
    }
}
