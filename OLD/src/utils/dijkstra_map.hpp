#pragma once

#include <vector>
#include <mutex>
#include "../components/position.hpp"

constexpr int16_t MAX_DIJSTRA_DISTANCE = 512;

struct dijkstra_map {
    dijkstra_map();
    void update(const std::vector<int> starting_points);
    void update_architecture(const std::vector<int> starting_points);
    position_t find_destination(const position_t &pos);
    int16_t get(const std::size_t &idx);

private:
    std::mutex map_lock;
    std::vector<int16_t> distance_map;
    void update_async(const std::vector<int> &starting_points);
    void update_architecture_async(const std::vector<int> &starting_points);
};
