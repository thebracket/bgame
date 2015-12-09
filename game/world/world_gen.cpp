#include "world_gen.hpp"
#include "world_height_map.hpp"
#include "world_gen_fault_lines.hpp"
#include "../../engine/rng.h"

void world_gen_phase_1()
{
    engine::random_number_generator rng;
  
    std::unique_ptr<heightmap_t> base_map = get_height_map();
    zero_height_map( base_map.get() );
    add_fault_lines( base_map.get(), &rng );
    
    // Smooth once
    // Hydrology
}
