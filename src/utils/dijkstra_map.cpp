#include "dijkstra_map.hpp"
#include "../main/game_globals.hpp"
#include <utility>
#include <queue>
#include <map>

dijkstra_map::dijkstra_map() {
    distance_map.resize(REGION_TILES_COUNT);
    std::fill(distance_map.begin(), distance_map.end(), MAX_DIJSTRA_DISTANCE);
}

void dijkstra_map::update(const std::vector<int> &starting_points)
{
    std::fill(distance_map.begin(), distance_map.end(), MAX_DIJSTRA_DISTANCE);

    // Populate the open list with starting points
    std::queue<std::pair<int,int>> open_nodes;
    for (const int &sp : starting_points) {
        open_nodes.emplace(std::make_pair(sp, 0));
    }

    // Iterate open nodes list
    while (!open_nodes.empty()) {
        const std::pair<int,int> open_node = open_nodes.front();
        open_nodes.pop();

        if (distance_map[open_node.first] > open_node.second && open_node.second < MAX_DIJSTRA_DISTANCE)
        {
            distance_map[open_node.first] = open_node.second;

            int x,y,z;
            std::tie(x,y,z) = idxmap(open_node.first);

            if (x < REGION_WIDTH-1 && current_region->tile_flags[open_node.first].test(CAN_GO_EAST)) {
                open_nodes.emplace(std::make_pair(mapidx(x+1, y, z), open_node.second+1));
            }
            if (x > 0 && current_region->tile_flags[open_node.first].test(CAN_GO_WEST)) {
                open_nodes.emplace(std::make_pair(mapidx(x-1, y, z), open_node.second+1));
            }
            if (y < REGION_WIDTH-1 && current_region->tile_flags[open_node.first].test(CAN_GO_SOUTH)) {
                open_nodes.emplace(std::make_pair(mapidx(x, y+1, z), open_node.second+1));
            }
            if (y > 0 && current_region->tile_flags[open_node.first].test(CAN_GO_WEST)) {
                open_nodes.emplace(std::make_pair(mapidx(x, y-1, z), open_node.second+1));
            }
            if (z > 0 && current_region->tile_flags[open_node.first].test(CAN_GO_DOWN)) {
                open_nodes.emplace(std::make_pair(mapidx(x, y, z-1), open_node.second+1));
            }
            if (z < REGION_DEPTH-1 && current_region->tile_flags[open_node.first].test(CAN_GO_UP)) {
                open_nodes.emplace(std::make_pair(mapidx(x, y, z+1), open_node.second+1));
            }
        }
    }
}

position_t dijkstra_map::find_destination(const position_t &pos) {
    const int idx = mapidx(pos);
    std::map<int16_t, int> candidates;
    if (pos.x > 0 && current_region->tile_flags[idx].test(CAN_GO_WEST)) {
        const int destidx = mapidx(pos.x-1, pos.y, pos.z);
        candidates.insert(std::make_pair(distance_map[destidx], destidx));
    }
    if (pos.x < REGION_WIDTH-1 && current_region->tile_flags[idx].test(CAN_GO_EAST)) {
        const int destidx = mapidx(pos.x+1, pos.y, pos.z);
        candidates.insert(std::make_pair(distance_map[destidx], destidx));
    }
    if (pos.y > 0 && current_region->tile_flags[idx].test(CAN_GO_NORTH)) {
        const int destidx = mapidx(pos.x, pos.y-1, pos.z);
        candidates.insert(std::make_pair(distance_map[destidx], destidx));
    }
    if (pos.y < REGION_HEIGHT-1 && current_region->tile_flags[idx].test(CAN_GO_SOUTH)) {
        const int destidx = mapidx(pos.x, pos.y+1, pos.z);
        candidates.insert(std::make_pair(distance_map[destidx], destidx));
    }
    if (pos.z > 0 && current_region->tile_flags[idx].test(CAN_GO_DOWN)) {
        const int destidx = mapidx(pos.x, pos.y, pos.z-1);
        candidates.insert(std::make_pair(distance_map[destidx], destidx));
    }
    if (pos.z < REGION_DEPTH-1 && current_region->tile_flags[idx].test(CAN_GO_UP)) {
        const int destidx = mapidx(pos.x, pos.y, pos.z+1);
        candidates.insert(std::make_pair(distance_map[destidx], destidx));
    }
    int dx, dy, dz;
    std::tie(dx, dy, dz) = idxmap(candidates.begin()->second);
    return position_t{dx, dy, dz};
}