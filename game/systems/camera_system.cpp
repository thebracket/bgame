#include "camera_system.h"
#include "../../engine/globals.h"

namespace engine {
namespace ecs {

void camera_system::tick(const double &duration_ms) {
    position_component * camera_pos = engine::globals::ecs->find_entity_component<position_component>(world::camera_handle);

    if ( command::is_key_down ( command::UP ) ) {
        camera_pos->y--;
        if (camera_pos->y < 0) camera_pos->y = 0;
    }
    if ( command::is_key_down ( command::DOWN ) ) {
        camera_pos->y++;
        if (camera_pos->y > landblock_height) camera_pos->y = landblock_height;
    }
    if ( command::is_key_down ( command::LEFT ) ) {
        camera_pos->x--;
        if (camera_pos->x < 0) camera_pos->x = 0;
    }
    if ( command::is_key_down ( command::RIGHT ) ) {
        camera_pos->x++;
        if (camera_pos->x > landblock_width) camera_pos->x = landblock_width;
    }
}

}
}