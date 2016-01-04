#include "main_menu.h"
#include "genesis_mode.hpp"
#include "game_mode.hpp"
#include <fstream>
#include "../engine/gui/gui_frame.h"
#include "../engine/gui/gui_static_text.h"
#include "game.h"
#include <string>

using std::make_unique;
using std::ifstream;
using namespace engine;

// the buttons are 406 x 79
// Therefore, left is 309
class main_menu_sg: public engine::base_node
{
	virtual void render(sdl2_backend * SDL)
	{
		const int mx = engine::command::mouse_x;
		const int my = engine::command::mouse_y;

		SDL_Color sdl_white =
		{ 255, 255, 255, 255 };
		SDL_Color sdl_yelllow =
		{ 255, 255, 0, 255 };

		const std::string btn_playgame = game_engine->render_text_to_image(
				"disco36", "Play The Game", "btn_playgame", sdl_white);
		const std::string btn_worldgen = game_engine->render_text_to_image(
				"disco36", "Generate The World", "btn_worldgen", sdl_white);
		const std::string btn_quit = game_engine->render_text_to_image(
				"disco36", "Exit The Game", "btn_quit", sdl_white);

		const std::string btn_playgame_a = game_engine->render_text_to_image(
				"disco36", "Play The Game", "btn_playgame_a", sdl_yelllow);
		const std::string btn_worldgen_a = game_engine->render_text_to_image(
				"disco36", "Generate The World", "btn_worldgen_a", sdl_yelllow);
		const std::string btn_quit_a = game_engine->render_text_to_image(
				"disco36", "Exit The Game", "btn_quit_a", sdl_yelllow);

		if (mx > 309 and mx < 715 and my > 250 and my < 329)
		{
			SDL->render_bitmap("big_button_2",
			{ 0, 0, 406, 79 },
			{ 309, 250, 406, 79 });
			SDL->render_bitmap_centered(btn_playgame_a, 512, 260);
		}
		else
		{
			SDL->render_bitmap("big_button_1",
			{ 0, 0, 406, 79 },
			{ 309, 250, 406, 79 });
			SDL->render_bitmap_centered(btn_playgame, 512, 260);
		}

		if (mx > 309 and mx < 715 and my > 350 and my < 429)
		{
			SDL->render_bitmap("big_button_2",
			{ 0, 0, 406, 79 },
			{ 309, 350, 406, 79 });
			SDL->render_bitmap_centered(btn_worldgen_a, 512, 360);
		}
		else
		{
			SDL->render_bitmap("big_button_1",
			{ 0, 0, 406, 79 },
			{ 309, 350, 406, 79 });
			SDL->render_bitmap_centered(btn_worldgen, 512, 360);
		}

		if (mx > 309 and mx < 715 and my > 450 and my < 529)
		{
			SDL->render_bitmap("big_button_2",
			{ 0, 0, 406, 79 },
			{ 309, 450, 406, 79 });
			SDL->render_bitmap_centered(btn_quit_a, 512, 460);
		}
		else
		{
			SDL->render_bitmap("big_button_1",
			{ 0, 0, 406, 79 },
			{ 309, 450, 406, 79 });
			SDL->render_bitmap_centered(btn_quit, 512, 460);
		}
	}
};

bool fexists(const char *filename)
{
	ifstream ifile(filename);
	if (ifile)
		return true;
	return false;
}

void main_menu::init()
{
	world_available = fexists("world/REGION_0.dat");
	sg.children.push_back(make_unique<engine::scene_background>("menuscreen"));
	sg.children.push_back(make_unique<main_menu_sg>());
}

void main_menu::done()
{

}

void main_menu::on_pop()
{
	world_available = fexists("world/LB1.dat");
}

pair<engine::return_mode, unique_ptr<engine::base_mode> > main_menu::tick(
		const double time_elapsed)
{
	if (engine::command::left_click)
	{
		const int mx = engine::command::mouse_x;
		const int my = engine::command::mouse_y;

		selected_item = 0;
		if (mx > 309 and mx < 715 and my > 250 and my < 329)
			selected_item = 1;
		if (mx > 309 and mx < 715 and my > 350 and my < 429)
			selected_item = 2;
		if (mx > 309 and mx < 715 and my > 450 and my < 529)
			selected_item = 3;

		if (selected_item == 1)
			return make_pair(PUSH, make_unique<game_mode>()); // Play the game
		if (selected_item == 2)
			return make_pair(PUSH, make_unique<genesis_mode>()); // Create the world!
		if (selected_item == 3)
			return make_pair(POP, NO_PUSHED_MODE); // Quit

				}
				//if ( engine::command::is_key_down( engine::command::Q ) ) return make_pair ( PUSH, make_unique<genesis_mode>() );
				//if ( engine::command::is_key_down( engine::command::P ) ) return make_pair ( PUSH, make_unique<game_mode>() );

				return make_pair ( CONTINUE, NO_PUSHED_MODE );
			}
