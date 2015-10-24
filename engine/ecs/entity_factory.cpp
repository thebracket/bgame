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


unique_ptr< base_component > construct_component_from_file ( fstream& lbfile )
{
    // Read the component type
    component_type ct;
    lbfile.read ( reinterpret_cast<char *> ( &ct ), sizeof ( ct ) );
    
    // Factory
    unique_ptr<base_component> component;
    
    switch (ct) {
      case position: component=make_unique<position_component>(); break;
      case name: component=make_unique<debug_name_component>(); break;
      case renderable: component=make_unique<renderable_component>(); break;
      case viewshed: component=make_unique<viewshed_component>(); break;
      case calendar: component=make_unique<calendar_component>(); break;
      case settler_ai: component=make_unique<settler_ai_component>(); break;
      case obstruction: component=make_unique<obstruction_component>(); break;
      default : {
	  throw 101;
      }
    }
    
    component->load(lbfile);
    return component; // Not std::move since we like return value optimization!
}

entity make_test_entity(const int &x, const int &y)
{
    entity test;
    test.handle = next_entity_handle();
    add_component(test, make_unique<debug_name_component>("Test"));
    add_component(test, make_unique<position_component>(x,y));
    add_component(test, make_unique<renderable_component>('@', yellow, black));
    add_component(test, make_unique<viewshed_component>(visibility,8));
    add_component(test, make_unique<settler_ai_component>());
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

entity make_cordex ( const int& x, const int& y, const long &system_time )
{
    entity cordex;
    cordex.handle = next_entity_handle();
    world::cordex_handle = cordex.handle;
    add_component(cordex, make_unique<debug_name_component>("Cordex"));
    add_component(cordex, make_unique<position_component>(x,y));
    add_component(cordex, unique_ptr<renderable_component>(new renderable_component(cyan, black, 3, 3, {177,30,177,17,15,16,177,31,177})));
    add_component(cordex, make_unique<viewshed_component>(penetrating,16));
    add_component(cordex, make_unique<calendar_component>(system_time));
    return cordex;
}
  
}
}
