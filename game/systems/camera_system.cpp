#include "camera_system.h"
#include "../game.h"

void camera_system::tick(const double &duration_ms) {
    position_component * camera_pos = game_engine->ecs->find_entity_component<position_component>(world::camera_handle);

    for (command_message &cmd : *game_engine->messaging->get_messages_by_type<command_message>() ) {
	  if (!cmd.deleted and cmd.command == CAMERA_UP) {
	      cmd.deleted = true;
	      camera_pos->y--;
	      if (camera_pos->y < 0) camera_pos->y = 0;
	  }
	  if (!cmd.deleted and cmd.command == CAMERA_DOWN) {
	      cmd.deleted = true;
	      camera_pos->y++;
	      if (camera_pos->y > landblock_height) camera_pos->y = landblock_height;
	  }
	  if (!cmd.deleted and cmd.command == CAMERA_LEFT) {
	      cmd.deleted = true;
	      camera_pos->x--;
	      if (camera_pos->x < 0) camera_pos->x = 0;
	  }
	  if (!cmd.deleted and cmd.command == CAMERA_RIGHT) {
	      cmd.deleted = true;
	      camera_pos->x++;
	      if (camera_pos->x > landblock_width) camera_pos->x = landblock_width;
	  }
     }    
}
