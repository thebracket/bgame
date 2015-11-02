#include "system_factory.h"
#include "camera_system.h"
#include "renderable_system.h"
#include "viewshed_system.h"
#include "calendar_system.h"
#include "settler_ai_system.h"
#include "obstruction_system.h"
#include "power_system.h"
#include "flowmap_system.h"
#include <memory>

using std::make_unique;
using std::unique_ptr;


unique_ptr<base_system> make_camera_system() { return make_unique<camera_system>(); }
unique_ptr<base_system> make_renderable_system() { return make_unique<renderable_system>(); }
unique_ptr<base_system> make_viewshed_system() { return make_unique<viewshed_system>(); }
unique_ptr<base_system> make_calendar_system() { return make_unique<calendar_system>(); }
unique_ptr<base_system> make_settler_ai_system() { return make_unique<settler_ai_system>(); }
unique_ptr<base_system> make_obstruction_system() { return make_unique<obstruction_system>(); }
unique_ptr<base_system> make_power_system() { return make_unique<power_system>(); }
unique_ptr< base_system > make_flowmap_system() { return make_unique<flowmap_system>(); }
