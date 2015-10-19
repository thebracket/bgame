#include "entity_factory.h"
#include "ecs.h"
#include "position_component.h"
#include "debug_name_component.h"
#include "renderable_component.h"
#include "../../game/world/world.h"
#include "../colors.h"

using std::make_unique;

namespace engine {
namespace ecs {

int entity_handle=0;

inline int next_entity_handle() {
    int result = entity_handle;
    ++entity_handle;
    return result;
}
  
entity make_test_entity(const int &x, const int &y)
{
    entity test;
    test.handle = next_entity_handle();
    add_component(test, make_unique<debug_name_component>("Test"));
    add_component(test, make_unique<position_component>(x,y));
    add_component(test, make_unique<renderable_component>('@', yellow, black));
    return test;
}

entity make_camera_entity()
{
    entity camera;
    camera.handle = next_entity_handle();
    world::camera_handle = camera.handle;
    add_component(camera, make_unique<position_component>(128,128));
    return camera;
}


  
}
}
