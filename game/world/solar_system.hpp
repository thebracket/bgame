#pragma once

#include <vector>
#include <cstdint>
#include <fstream>
#include "solar_system_body.hpp"

/*
 * Represents a solar system in the universe.
 */
struct solar_system_t {
    /* Index location in the universe. */
    uint16_t universe_idx;
    
    /* X location in the universe. */
    uint8_t universe_x;
    
    /* Y location in the universe. */
    uint8_t universe_y;
    
    /* Vector of bodies in the solar system. */
    std::vector<solar_system_body_t> bodies;
    
    // Serialization
    void save( std::fstream &lbfile ) const;
    void load( std::fstream &lbfile );
};
