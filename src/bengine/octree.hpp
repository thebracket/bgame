#pragma once

#include <vector>
#include <memory>
#include "../planet/region/region.hpp"

struct octree_location_t {
    int x, y, z;
    int id;
};

// Not really an octree anymore - trying to speed it up
struct octree_t {
    octree_t() {
        contents.resize(REGION_TILES_COUNT);
    }

    std::vector<std::vector<int>> contents;
    std::size_t total_nodes = 0;

    void add_node(const octree_location_t loc);

    void remove_node(const octree_location_t &loc);

    std::vector<int> find_by_loc(const octree_location_t &loc);

    std::vector<int> find_by_region(const int &left, const int &right, const int &top, const int &bottom,
                                            const int &ztop, const int &zbottom);
};
