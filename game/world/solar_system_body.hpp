#pragma once

#include <cstdint>
#include <memory>

/*
 * Represents a planet or other body orbiting a star system.
 */
struct solar_system_body_t {
    /* Index within the solar system. */
    uint8_t system_idx;
        
    // Serialization
    void save( std::fstream &lbfile ) const;
    void load( std::fstream &lbfile );
};
