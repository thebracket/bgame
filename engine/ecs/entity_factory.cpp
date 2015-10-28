#include "entity_factory.h"
#include "ecs.h"
#include "../../game/world/world.h"
#include "../colors.h"
#include "../globals.h"

using std::make_unique;

namespace engine {
namespace ecs {

int entity_handle=0;

int next_entity_handle() {
    int result = entity_handle;
    ++entity_handle;
    return result;
}

entity make_test_entity(const int &x, const int &y)
{
    entity test;
    test.handle = next_entity_handle();
    engine::globals::ecs->add_component(test, debug_name_component("Test"));
    engine::globals::ecs->add_component(test, position_component(x,y));
    engine::globals::ecs->add_component(test, renderable_component('@', yellow, black));
    engine::globals::ecs->add_component(test, viewshed_component(visibility,8));
    engine::globals::ecs->add_component(test, settler_ai_component());
    return test;
}

entity make_camera_entity()
{
    entity camera;
    camera.handle = next_entity_handle();
    world::camera_handle = camera.handle;
    engine::globals::ecs->add_component(camera, position_component(128,128));
    return camera;
}

}
}
