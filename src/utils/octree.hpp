#pragma once

#include <array>
#include <vector>
#include <memory>

struct octree_location_t {
    int x, y, z;
    std::size_t id;
};

struct octree_t {
    int x, y, z;
    int w, h, d, n;
    int total_nodes = 0;
    int half_w, half_h, half_d;

    std::array<std::unique_ptr<octree_t>, 8> children;
    std::vector<octree_location_t> contents;

    octree_t(const int &width, const int &height, const int &depth, const int layer=0);
    octree_t(const octree_t * parent, int section);

    inline int get_child_index(const octree_location_t &loc) {
        if (loc.x < half_w && loc.x < half_h && loc.z < half_d) return 0; // Upper top-left
        if (loc.x > half_w && loc.x < half_h && loc.z < half_d) return 1; // Upper top-right
        if (loc.x < half_w && loc.x > half_h && loc.z < half_d) return 2; // Upper bottom-left
        if (loc.x > half_w && loc.x > half_h && loc.z < half_d) return 3; // Upper bottom-right
        if (loc.x < half_w && loc.x < half_h && loc.z > half_d) return 4; // Lower top-left
        if (loc.x > half_w && loc.x < half_h && loc.z > half_d) return 5; // Lower top-right
        if (loc.x < half_w && loc.x > half_h && loc.z > half_d) return 6; // Lower bottom-left
        return 7; // Lower bottom-right
    }

    void add_node(const octree_location_t loc);
    int find_nearest(const octree_location_t &loc);
    void remove_node(const octree_location_t &loc);
    std::vector<std::size_t> find_by_loc(const octree_location_t &loc);

private:
    void find_by_loc_impl(const octree_location_t &loc, std::vector<std::size_t> &result);
};
