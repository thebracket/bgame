#include "system_factory.h"
#include "camera_system.h"
#include "renderable_system.h"
#include "viewshed_system.h"
#include "calendar_system.h"
#include "settler_ai_system.h"
#include "obstruction_system.h"
#include "power_system.h"
#include "input_system.h"
#include "inventory_system.h"
#include "cordex_ai_system.h"
#include "damage_system.h"
#include "particle_system.h"
#include "global_illumination_system.hpp"
#include <memory>

using std::make_unique;
using std::unique_ptr;

unique_ptr<base_system> make_camera_system()
{
	return make_unique<camera_system>();
}
unique_ptr<base_system> make_renderable_system()
{
	return make_unique<renderable_system>();
}
unique_ptr<base_system> make_viewshed_system()
{
	return make_unique<viewshed_system>();
}
unique_ptr<base_system> make_calendar_system()
{
	return make_unique<calendar_system>();
}
unique_ptr<base_system> make_settler_ai_system()
{
	return make_unique<settler_ai_system>();
}
unique_ptr<base_system> make_obstruction_system()
{
	return make_unique<obstruction_system>();
}
unique_ptr<base_system> make_power_system()
{
	return make_unique<power_system>();
}
unique_ptr<base_system> make_input_system()
{
	return make_unique<input_system>();
}
unique_ptr<base_system> make_inventory_system()
{
	return make_unique<inventory_system>();
}
unique_ptr<base_system> make_cordex_ai_system()
{
	return make_unique<cordex_ai_system>();
}
unique_ptr<base_system> make_damage_system()
{
	return make_unique<damage_system>();
}
unique_ptr<base_system> make_particle_system()
{
	return make_unique<particle_system>();
}
unique_ptr<base_system> make_global_illumination_system()
{
	return make_unique<global_illumination_system>();
}
