#pragma once

#include "../components/position.hpp"
#include <tuple>
#include "constants.hpp"
#include "../utils/system_log.hpp"

#define BOUNDS_CHECKING 1

/*
 * Calculate the array index of an x/y/z position.
 */
inline int mapidx(const int &x, const int &y, const int &z) noexcept {
    return (z * REGION_HEIGHT * REGION_WIDTH) + (y * REGION_WIDTH) + x;
}

/*
 * Calculate the array index of a position component
 */
inline int mapidx(const position_t &pos) noexcept {
    return mapidx(static_cast<int>(pos.x), static_cast<int>(pos.y), pos.z);
}

/*
 * Reverse an array index back into an x/y/z position
 */
inline std::tuple<int,int,int> idxmap(int idx) noexcept {
    int z = idx / (REGION_HEIGHT * REGION_WIDTH);
    idx -= (z * REGION_WIDTH * REGION_HEIGHT);

    int y = idx / REGION_WIDTH;
    idx -= (y * REGION_WIDTH);

    int x = idx;

    return std::make_tuple(x,y,z);
}
