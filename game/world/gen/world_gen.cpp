#include "world_gen.hpp"
#include "world_height_map.hpp"
#include "world_gen_fault_lines.hpp"
#include "world_gen_hydrology.hpp"
#include "../../engine/rng.h"
#include "world_gen_layer_cake.hpp"
#include <iostream>
#include "../game.h"
#include "universe.hpp"

using engine::entity;

void world_gen_phase_1()
{
    std::cout << "World gen running\n";
    engine::random_number_generator rng;
  
    std::unique_ptr<heightmap_t> base_map = get_height_map();
    std::cout << "Zero Height Map\n";
    zero_height_map( base_map.get() );
    std::cout << "Fault Lines\n";
    add_fault_lines( base_map.get(), &rng );
    //std::cout << "Hydrology\n";
    //std::unique_ptr<water_level_map_t> water = perform_hydrology( base_map.get(), &rng );
    std::cout << "Smooth\n";
    for (int i=0; i<128; ++i)
	smooth_height_map( base_map.get() );    
    std::cout << "Layercake\n";
    std::unique_ptr<planet_t> planet = make_world_layers( base_map.get(), rng );
    std::cout << "Making starting entites\n";
    
    // Make the camera
    entity camera = game_engine->ecs->add_entity();
    game_engine->ecs->add_component(camera, position_component3d({ planet->planet_idx( WORLD_WIDTH/2, WORLD_HEIGHT-1), REGION_WIDTH/2, REGION_HEIGHT/2, 32 }));
    universe->globals.camera_handle = camera.handle;
    world::camera_handle = camera.handle;
    game_engine->ecs->save_game("world/savegame3d.dat");
    
    std::cout << "World gen done\n";
}
