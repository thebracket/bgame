#pragma once

#include <vector>
#include <cstdint>
#include <fstream>
#include <utility>
#include "solar_system_body.hpp"
#include "../../engine/sdl2_backend.hpp"

// See: https://en.wikipedia.org/wiki/Stellar_classification
enum system_class_t { O, B, A, F, G, K, M };
enum system_control_t { NO_OWNER, ARSAK, CARVAZ, HERITEZ, LAMINTREZ, ERENTAR, SYL };

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
    
    /* Type of system */
    system_class_t system_class;
    bool binary_system;
    
    /* Vector of bodies in the solar system. */
    std::vector<solar_system_body_t> bodies;
    
    /* Has Eden reached this system yet? */
    bool inhabited_by_man = false;
    
    /* Which noble house owns it? */
    system_control_t owner = NO_OWNER;
    
    // Serialization
    void save( std::fstream &lbfile ) const;
    void load( std::fstream &lbfile );
};

std::pair<SDL_Color, unsigned char> solar_system_renderable( const solar_system_t &system);
