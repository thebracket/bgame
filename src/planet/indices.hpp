#pragma once

#include "../components/position.hpp"
#include <tuple>
#include <assert.h>
#include "constants.hpp"

#define BOUNDS_CHECKING 1

/*
 * Calculate the array index of an x/y/z position.
 */
inline int mapidx(const int &x, const int &y, const int &z) noexcept {
#ifdef BOUNDS_CHECKING
    if (x < 0 || x > REGION_WIDTH-1 || y < 0 || y>REGION_HEIGHT-1 || z<0 || z>REGION_DEPTH+1) {
        std::cout << "MapIdx bounds check fail: " << x << "/" << y << "/" << z << "\n";
        assert(x>-1 && x<REGION_WIDTH && y>-1 && y<REGION_HEIGHT && z>-1 && z<REGION_DEPTH);
    }
#endif
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

#ifdef BOUNDS_CHECKING
    if (x < 0 || x > REGION_WIDTH-1 || y < 0 || y>REGION_HEIGHT-1 || z<0 || z>REGION_DEPTH+1) {
        std::cout << "MapIdx bounds check fail: " << x << "/" << y << "/" << z << "\n";
        //assert(x>-1 && x<REGION_WIDTH && y>-1 && y<REGION_HEIGHT && z>-1 && z<REGION_DEPTH);
    }
#endif
    return std::make_tuple(x,y,z);
}
