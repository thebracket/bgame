#include "camera_system.h"
#include "../game.h"
#include "../world/region.hpp"

void camera_system::tick(const double &duration_ms) {
    position_component3d * camera_pos = game_engine->ecs->find_entity_component<position_component3d>(world::camera_handle);

    for (command_message &cmd : *game_engine->messaging->get_messages_by_type<command_message>() ) {
	  if (!cmd.deleted and cmd.command == CAMERA_UP) {
	      cmd.deleted = true;
	      camera_pos->pos.y--;
	      if (camera_pos->pos.y < 0) camera_pos->pos.y = 0;
	      game_engine->messaging->add_message<entity_moved_message>( entity_moved_message() );
	  }
	  if (!cmd.deleted and cmd.command == CAMERA_DOWN) {
	      cmd.deleted = true;
	      camera_pos->pos.y++;
	      if (camera_pos->pos.y > landblock_height) camera_pos->pos.y = REGION_HEIGHT;
	      game_engine->messaging->add_message<entity_moved_message>( entity_moved_message() );
	  }
	  if (!cmd.deleted and cmd.command == CAMERA_LEFT) {
	      cmd.deleted = true;
	      camera_pos->pos.x--;
	      if (camera_pos->pos.x < 0) camera_pos->pos.x = 0;
	      game_engine->messaging->add_message<entity_moved_message>( entity_moved_message() );
	  }
	  if (!cmd.deleted and cmd.command == CAMERA_RIGHT) {
	      cmd.deleted = true;
	      camera_pos->pos.x++;
	      if (camera_pos->pos.x > landblock_width) camera_pos->pos.x = REGION_WIDTH;
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
