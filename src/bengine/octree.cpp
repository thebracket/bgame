#include "octree.hpp"
#include <algorithm>

void octree_t::add_node(const octree_location_t loc) {
    const auto idx = mapidx(loc.x, loc.y, loc.z);
    contents[idx].emplace_back(loc.id);
    ++total_nodes;
}

void octree_t::remove_node(const octree_location_t &loc) {
    const auto idx = mapidx(loc.x, loc.y, loc.z);
    contents[idx].erase(
        std::remove_if(
                contents[idx].begin(),
                contents[idx].end(),
                [&loc] (const std::size_t &test) { return test == loc.id; }
        ),
        contents[idx].end());
}

std::vector<int> octree_t::find_by_loc(const octree_location_t &loc) {
    std::vector<int> result;
    const auto idx = mapidx(loc.x, loc.y, loc.z);
    for (const auto &loc : contents[idx]) {
        result.push_back(loc);
    }
    return result;
}

std::vector<int> octree_t::find_by_region(const int &left, const int &right, const int &top, const int &bottom,
                                        const int &ztop, const int &zbottom)
{
    std::vector<int> result;
    for (auto z=zbottom; z<ztop; ++z) {
        for (auto y=top; y<bottom; ++y) {
            for (auto x=left; x<right; ++x) {
                const auto idx = mapidx(x,y,z);
                for (const auto &loc : contents[idx]) {
                    result.emplace_back(loc);
                }
            }
        }
    }
    return result;
}
