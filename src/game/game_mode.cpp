#include "game_mode.hpp"
#include "../engine/command_manager.hpp"
#include "world/planet.hpp"
#include "world/world_defs.hpp"
#include "../engine/virtual_terminal.hpp"
#include "../game/game.h"
#include "world/universe.hpp"
#include "render/colors.h"
#include "render/render_list.hpp"
#include "world/tile_types.hpp"
#include <utility>
#include <iomanip>
#include <sstream>
#include "systems/system_factory.hpp"

using std::make_pair;
using namespace engine;
using namespace engine::command;
using engine::vterm::color_t;
using std::make_unique;
using engine::vterm::desaturate;
using engine::vterm::darken;
using engine::vterm::apply_colored_light;

void game_mode::init_systems()
{
	ECS->add_system(make_input_system());
	ECS->add_system(make_camera_system());
	ECS->add_system(make_calendar_system());
	ECS->add_system(make_obstruction_system());
	ECS->add_system(make_inventory_system());
	ECS->add_system(make_power_system());
	ECS->add_system(make_cordex_ai_system());
	ECS->add_system(make_settler_ai_system());
	ECS->add_system(make_damage_system());
	ECS->add_system(make_particle_system());
	ECS->add_system(make_viewshed_system());
	ECS->add_system(make_renderable_system());
	ECS->add_system(make_global_illumination_system());
	ECS->add_system(make_hud_render_system());
	ECS->add_system(make_map_render_system());
	ECS->add_system(make_gui_render_system());
	ECS->add_system(make_entity_cleanup_system());
}

void game_mode::init()
{
	finished = false;
	world::planet->load_region(world::planet->planet_idx(WORLD_WIDTH / 2, WORLD_HEIGHT - 1));

	ECS->init();
	ECS->load_game("world/savegame3d.dat");
	init_systems();

	// Tell the rendering and movement systems to fire
	game_engine->messaging->add_message<walkability_changed_message>(walkability_changed_message());
	game_engine->messaging->add_message<entity_moved_message>(entity_moved_message());
}

void game_mode::done()
{
	ECS->save_game("world/savegame3d.dat");
	world::planet->save_region(world::planet->planet_idx(WORLD_WIDTH / 2, WORLD_HEIGHT - 1));
	ECS->done();
}

pair<engine::return_mode, unique_ptr<engine::base_mode> > game_mode::tick(
		const double time_elapsed)
{
	// Temporary
	if (is_key_down(Q))
		finished = true;
	if (is_key_down(command::TILDE))
	{
		vector<tuple<string, int, int, int>> timings = ECS->get_profile_info();
		std::cout << std::setw(20) << "System" << std::setw(10) << "Last" << std::setw(10) << "Min" << std::setw(10) << "Max\n";
		std::cout << "--------------------------------------------------\n";
		for (auto line : timings)
		{
			std::cout << std::setw(20) << std::get<0>(line) << std::setw(10)
					<< std::get<1>(line) << std::setw(10) << std::get<2>(line)
					<< std::setw(10) << std::get<3>(line) << "\n";
		}
		std::cout << "--------------------------------------------------\n";
	}
	if (is_key_down(F10))
	{
		universe->globals.omniscience = !universe->globals.omniscience;
	}

	if (finished)
	{
		return make_pair(POP, NO_PUSHED_MODE);
	}
	else
	{
		return make_pair ( CONTINUE, NO_PUSHED_MODE );
	}
}
