#pragma once

#include <cstdint>

/*
 * Defines a location in space.
 */
struct location_t {
    /* The index of the region to which the point belongs. */
    uint16_t region;
    
    /* X coordinate. */
    uint8_t x;
    
    /* Y coordinate. */
    uint8_t y;
    
    /* Z coordinate. */
    uint8_t z;
};
