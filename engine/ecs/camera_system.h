#pragma once

#include "base_system.h"
#include "../../game/world/world.h"
#include "ecs.h"
#include "position_component.h"
#include "../command_manager.h"

namespace engine {
namespace ecs {

class camera_system : public base_system {
    virtual void tick(const double &duration_ms) {
        const entity * camera = get_entity_by_handle(world::camera_handle);
        position_component * camera_pos = static_cast<position_component *>(
                get_component_by_handle(camera->component_handles[0])
                                                );

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
};

}
}