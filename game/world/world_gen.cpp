#include "world_gen.hpp"
#include "world_height_map.hpp"
#include "world_gen_fault_lines.hpp"
#include "world_gen_hydrology.hpp"
#include "planet.hpp"
#include "../../engine/rng.h"
#include "world_gen_layer_cake.hpp"
#include <iostream>

void world_gen_phase_1()
{
    std::cout << "World gen running\n";
    engine::random_number_generator rng;
  
    std::unique_ptr<heightmap_t> base_map = get_height_map();
    std::cout << "Zero Height Map\n";
    zero_height_map( base_map.get() );
    std::cout << "Fault Lines\n";
    add_fault_lines( base_map.get(), &rng );
    std::cout << "Smooth\n";
    smooth_height_map( base_map.get() );
    smooth_height_map( base_map.get() );
    smooth_height_map( base_map.get() );
    smooth_height_map( base_map.get() );
    std::cout << "Hydrology\n";
    std::unique_ptr<water_level_map_t> water = perform_hydrology( base_map.get(), &rng );
    std::cout << "Layercake\n";
    make_world_layers( base_map.get(), rng );
    std::cout << "World gen done\n";
}
