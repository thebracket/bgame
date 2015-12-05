#include "solar_system.hpp"

void solar_system_t::load( std::fstream &lbfile )
{

}

void solar_system_t::save( std::fstream &lbfile ) const
{

}

std::pair<SDL_Color, unsigned char> solar_system_renderable( const solar_system_t &system)
{
    return std::make_pair( SDL_Color{255,255,255,0}, '*' );
}
