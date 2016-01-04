#pragma once

#include <cstdint>

/*
 * Defines a location in space.
 */
struct location_t {
    /* The index of the region to which the point belongs. */
    uint8_t region;
    
    /* X coordinate. */
    int16_t x;
    
    /* Y coordinate. */
    int16_t y;
    
    /* Z coordinate. */
    uint8_t z;
};
