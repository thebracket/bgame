#pragma once

#include "../../engine/ecs.h"
#include <memory>

using std::unique_ptr;

unique_ptr<base_system> make_camera_system();
unique_ptr<base_system> make_renderable_system();
unique_ptr<base_system> make_viewshed_system();
unique_ptr<base_system> make_calendar_system();
unique_ptr<base_system> make_settler_ai_system();
unique_ptr<base_system> make_obstruction_system();
unique_ptr<base_system> make_power_system();
unique_ptr<base_system> make_flowmap_system();
