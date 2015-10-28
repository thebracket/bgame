#include "camera_system.h"
#include "../../globals.h"

namespace engine {
namespace ecs {

void camera_system::tick(const double &duration_ms) {
    const entity * camera = engine::globals::ecs->get_entity_by_handle(world::camera_handle);
    position_component * camera_pos = engine::globals::ecs->get_component_by_handle<position_component>(camera->find_component_by_type(ecs::position));

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