#pragma once

#include "universe.hpp"

class universe_generator {
public:
     universe_generator();
     void make_universe();
     void spread_humanity();
     void spread_ownership();

private:
     void detail_star_system ( solar_system_t & system );
     void add_planetary_bodies ( solar_system_t & system );
     void add_planetary_body ( int i, solar_system_t &system );
};
