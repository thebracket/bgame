#include "entity_factory.h"
#include "ecs.h"
#include "../../game/world/world.h"
#include "../colors.h"

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
    add_component(test, debug_name_component("Test"));
    add_component(test, position_component(x,y));
    add_component(test, renderable_component('@', yellow, black));
    add_component(test, viewshed_component(visibility,8));
    add_component(test, settler_ai_component());
    return test;
}

entity make_camera_entity()
{
    entity camera;
    camera.handle = next_entity_handle();
    world::camera_handle = camera.handle;
    add_component(camera, position_component(128,128));
    return camera;
}

}
}
