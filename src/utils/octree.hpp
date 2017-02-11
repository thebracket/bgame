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

    std::array<std::unique_ptr<octree_t>, 8> children;
    std::vector<octree_location_t> contents;

    octree_t(const int &width, const int &height, const int &depth, const int layer=0);
    octree_t(const octree_t * parent, int section);

    inline int get_child_index(const octree_location_t &loc) {
		const int half_w = x+(w/2);
		const int half_h = y+(h/2);
		const int half_d = z+(d/2);
	
        if (loc.x < half_w && loc.x < half_h && loc.z < half_d) return 0; // Upper top-left
        if (loc.x > half_w && loc.x < half_h && loc.z < half_d) return 1; // Upper top-right
        if (loc.x < half_w && loc.x > half_h && loc.z < half_d) return 2; // Upper bottom-left
        if (loc.x > half_w && loc.x > half_h && loc.z < half_d) return 3; // Upper bottom-right
        if (loc.x < half_w && loc.x < half_h && loc.z > half_d) return 4; // Lower top-left
        if (loc.x > half_w && loc.x < half_h && loc.z > half_d) return 5; // Lower top-right
        if (loc.x < half_w && loc.x > half_h && loc.z > half_d) return 6; // Lower bottom-left
        return 7; // Lower bottom-right
    }

    inline bool intersects(const int &segment, const int &left, const int &right, const int &top, const int &bottom, const int &ztop, const int &zbottom) {
        const int half_w = x+(w/2);
        const int half_h = y+(h/2);
        const int half_d = z+(d/2);

        switch (segment) {
            case 0 : {
                // Upper top-left
                if ((left <= half_w || right <= half_w) && (top <= half_h || bottom <= half_h) && (ztop <= half_d || zbottom <= half_d))
                    return true;
            } break;
            case 1 : {
                // Upper top-right
                if ((left >= half_w || right >= half_w) && (top <= half_h || bottom <= half_h) && (ztop <= half_d || zbottom <= half_d))
                    return true;
            } break;
            case 2 : {
                // Upper bottom-left
                if ((left <= half_w || right <= half_w) && (top >= half_h || bottom >= half_h) && (ztop <= half_d || zbottom <= half_d))
                    return true;
            } break;
            case 3 : {
                // Upper bottom-right
                if ((left >= half_w || right >= half_w) && (top >= half_h || bottom >= half_h) && (ztop <= half_d || zbottom <= half_d))
                    return true;
            } break;
            case 4 : {
                // Lower top-left
                if ((left <= half_w || right <= half_w) && (top <= half_h || bottom <= half_h) && (ztop >= half_d || zbottom >= half_d))
                    return true;
            } break;
            case 5 : {
                // Lower top-right
                if ((left >= half_w || right >= half_w) && (top <= half_h || bottom <= half_h) && (ztop >= half_d || zbottom >= half_d))
                    return true;
            } break;
            case 6 : {
                // Lower bottom-left
                if ((left <= half_w || right <= half_w) && (top >= half_h || bottom >= half_h) && (ztop >= half_d || zbottom >= half_d))
                    return true;
            } break;
            case 7 : {
                // Lower bottom-right
                if ((left >= half_w || right >= half_w) && (top >= half_h || bottom >= half_h) && (ztop >= half_d || zbottom >= half_d))
                    return true;
            } break;
        }
        return false;
    }

    void add_node(const octree_location_t loc);
    int find_nearest(const octree_location_t &loc);
    void remove_node(const octree_location_t &loc);
    std::vector<std::size_t> find_by_loc(const octree_location_t &loc);
    std::vector<std::size_t> find_by_region(const int &left, const int &right, const int &top, const int &bottom, const int &ztop, const int &zbottom);

private:
    void find_by_loc_impl(const octree_location_t &loc, std::vector<std::size_t> &result);
    void find_by_region_impl(const int &left, const int &right, const int &top, const int &bottom, const int &ztop, const int &zbottom, std::vector<std::size_t> &result);
};
