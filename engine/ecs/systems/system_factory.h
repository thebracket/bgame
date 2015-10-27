#pragma once

#include "base_system.h"
#include <memory>

using std::unique_ptr;

namespace engine {
namespace ecs {

unique_ptr<base_system> make_camera_system();
unique_ptr<base_system> make_renderable_system();
unique_ptr<base_system> make_viewshed_system();
unique_ptr<base_system> make_calendar_system();
unique_ptr<base_system> make_settler_ai_system();
unique_ptr<base_system> make_obstruction_system();
unique_ptr<base_system> make_power_system();

}
}
