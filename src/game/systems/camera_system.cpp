#include "camera_system.h"
#include "../game.h"
#include "../world/region.hpp"

void camera_system::tick(const double &duration_ms) {
    position_component3d * camera_pos = game_engine->ecs->find_entity_component<position_component3d>(world::camera_handle);

    std::pair<int,int> size = engine::vterm::get_screen_size();
    const int width = size.first/8;
    const int height = size.second/8;
    
    for (command_message &cmd : *game_engine->messaging->get_messages_by_type<command_message>() ) {
	  if (!cmd.deleted and cmd.command == CAMERA_UP) {
	      cmd.deleted = true;
	      if (camera_pos->pos.y < height) {
		camera_pos->pos.y = height;
	      } else {
		camera_pos->pos.y--;
	      }
	      game_engine->messaging->add_message<entity_moved_message>( entity_moved_message() );
	  }
	  if (!cmd.deleted and cmd.command == CAMERA_DOWN) {
	      cmd.deleted = true;
	      camera_pos->pos.y++;
	      if (camera_pos->pos.y > REGION_HEIGHT-(height/2)) camera_pos->pos.y = REGION_HEIGHT-(height/2);
	      game_engine->messaging->add_message<entity_moved_message>( entity_moved_message() );
	  }
	  if (!cmd.deleted and cmd.command == CAMERA_LEFT) {
	      cmd.deleted = true;
	      camera_pos->pos.x--;
	      if (camera_pos->pos.x < width/2) camera_pos->pos.x = width/2;
	      game_engine->messaging->add_message<entity_moved_message>( entity_moved_message() );
	  }
	  if (!cmd.deleted and cmd.command == CAMERA_RIGHT) {
	      cmd.deleted = true;
	      camera_pos->pos.x++;
	      if (camera_pos->pos.x > REGION_WIDTH-(width/2)) camera_pos->pos.x = REGION_WIDTH-(width/2);
	      game_engine->messaging->add_message<entity_moved_message>( entity_moved_message() );
	  }
	  if (!cmd.deleted and cmd.command == CAMERA_Z_UP) {
	      cmd.deleted = true;
	      camera_pos->pos.z++;
	      if (camera_pos->pos.z > REGION_DEPTH) camera_pos->pos.z = REGION_DEPTH;
	      game_engine->messaging->add_message<entity_moved_message>( entity_moved_message() );
	  }
	  if (!cmd.deleted and cmd.command == CAMERA_Z_DOWN) {
	      cmd.deleted = true;
	      camera_pos->pos.z--;
	      if (camera_pos->pos.z < 1) camera_pos->pos.z = 1;
	      game_engine->messaging->add_message<entity_moved_message>( entity_moved_message() );
	  }
     }    
}
