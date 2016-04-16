#include "system_factory.hpp"

#include "global_illumination_system.hpp"
#include "entity_cleanup_system.hpp"
#include <memory>
#include "calendar_system.hpp"
#include "camera_system.hpp"
#include "cordex_ai_system.hpp"
#include "damage_system.hpp"
#include "gui_render_system.hpp"
#include "hud_render_system.hpp"
#include "input_system.hpp"
#include "inventory_system.hpp"
#include "map_render_system.hpp"
#include "obstruction_system.hpp"
#include "particle_system.hpp"
#include "power_system.hpp"
#include "renderable_system.hpp"
#include "settler_ai_system.hpp"
#include "viewshed_system.hpp"

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
unique_ptr<base_system> make_hud_render_system()
{
	return make_unique<hud_render_system>();
}
unique_ptr<base_system> make_map_render_system()
{
	return make_unique<map_render_system>();
}
unique_ptr<base_system> make_gui_render_system()
{
	return make_unique<gui_render_system>();
}
unique_ptr<base_system> make_entity_cleanup_system()
{
	return make_unique<entity_cleanup_system>();
}
