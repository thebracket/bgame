#pragma once

#include <vector>
#include "../components/position.hpp"

constexpr int16_t MAX_DIJSTRA_DISTANCE = 512;

struct dijkstra_map {
    dijkstra_map();
    void update(const std::vector<int> &starting_points);
    std::vector<int16_t> distance_map;
    position_t find_destination(const position_t &pos);
};
