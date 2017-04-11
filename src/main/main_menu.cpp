#include "main_menu.hpp"
#include <rltk.hpp>
#include <filesystem.hpp>
#include <algorithm>
#include "menu_helper.hpp"
#include "constants.hpp"
#include "game_globals.hpp"
#include "../systems/gui/imgui_helper.hpp"
#include "../external/imgui-sfml/imgui-SFML.h"
#include "../utils/filesystem.hpp"

constexpr int BACKDROP_LAYER=1;
constexpr int LOG_LAYER=2;

using namespace rltk;
using namespace rltk::colors;

std::pair<int,std::string> main_menu::get_descriptive_noun(const int do_not_use) const {
	random_number_generator rng;
	int roll = do_not_use;
	while (roll == do_not_use) {
		roll = rng.roll_dice(1,17);
	}
	switch (roll) {
		case 1 : return std::make_pair(1, std::string("Stupidity"));
		case 2 : return std::make_pair(2, std::string("Idiocy"));
		case 3 : return std::make_pair(3, std::string("Dullness"));
		case 4 : return std::make_pair(4, std::string("Foolishness"));
		case 5 : return std::make_pair(5, std::string("Futility"));
		case 6 : return std::make_pair(6, std::string("Naievity"));
		case 7 : return std::make_pair(7, std::string("Senselessness"));
		case 8 : return std::make_pair(8, std::string("Shortsightedness"));
		case 9 : return std::make_pair(9, std::string("Triviality"));
		case 10 : return std::make_pair(10, std::string("Brainlessness"));
		case 11 : return std::make_pair(11, std::string("Inanity"));
		case 12 : return std::make_pair(12, std::string("Insensitivity"));
		case 13 : return std::make_pair(13, std::string("Indiscretion"));
		case 14 : return std::make_pair(14, std::string("Mindlessness"));
		case 15 : return std::make_pair(15, std::string("Moronism"));
		case 16 : return std::make_pair(16, std::string("Obtuseness"));
		case 17 : return std::make_pair(17, std::string("Unthinkingness"));
	}
	return std::make_pair(18, "Bugginess");
}

void main_menu::init() {
	auto window_size = get_window()->getSize();
	const int window_width = window_size.x;
	const int window_height = window_size.y;

	gui->add_owner_layer(BACKDROP_LAYER, 0, 0, window_width, window_height, resize_fullscreen, draw_splash_backdrop);
	gui->add_layer(LOG_LAYER, 0, 0, window_width, window_height, game_config.gui_font, resize_fullscreen, false);

	if (exists(get_save_path() + std::string("/savegame.dat"))) {
		world_exists = true;
		selected = 0;
	}

	random_number_generator rng;
	switch (rng.roll_dice(1,8)) {
		case 1 : tagline = "Histories "; break;
		case 2 : tagline = "Chronicles "; break;
		case 3 : tagline = "Sagas "; break;
		case 4 : tagline = "Annals "; break;
		case 5 : tagline = "Narratives "; break;
		case 6 : tagline = "Recitals "; break;
		case 7 : tagline = "Tales "; break;
		case 8 : tagline = "Stories "; break;
	}

	auto first_noun = get_descriptive_noun();
	auto second_noun = get_descriptive_noun(first_noun.first);

	tagline += "of " + first_noun.second + " and " + second_noun.second;

	strncpy(online_username, game_config.online_username.c_str(), 254);
}

void main_menu::destroy() {
	gui->delete_layer(BACKDROP_LAYER);
	gui->delete_layer(LOG_LAYER);
	clicked = false;
}

void main_menu::tick(const double duration_ms) {
	if (key_delay < 1.0 && is_window_focused()) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			--selected;
			if (selected < 0) selected = 2;
			if (!world_exists && selected == 0) selected = 2;
			key_delay = 5.0;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			++selected;
			if (selected > 2) selected = 0;
			if (!world_exists && selected == 0) selected = 1;
			key_delay = 5.0;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
			clicked = true;
		}
	} else {
		key_delay -= duration_ms;
	}

	const int y_height = term(LOG_LAYER)->term_height;
	const int y_center = y_height / 2;

	term(LOG_LAYER)->clear();
	term(LOG_LAYER)->print_center(y_center - 6, VERSION, WHITE, BLACK);
	term(LOG_LAYER)->print_center(y_center - 5, tagline, LIGHT_RED, BLACK);
	term(LOG_LAYER)->print_center(y_center + 5, "Powered by RLTK - the RogueLike Tool Kit", RED, BLACK);
	term(LOG_LAYER)->print_center(y_center + 6, "http://www.bracketproductions.com/", YELLOW, BLACK);
	term(LOG_LAYER)->print_center(term(LOG_LAYER)->term_height-2, "To Kylah of the West, the Bravest Little Warrior of Them All", WHITE, BLACK);

    if (!show_options) {
        ImGui::SetNextWindowPosCenter();
        ImGui::Begin("Startup", nullptr, ImVec2{400, 400}, 0.0f,
                     ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse + ImGuiWindowFlags_NoTitleBar);
        if (world_exists) {
            if (ImGui::Button("Play the Game")) {
                selected = 0;
                clicked = true;
            }
        }
        if (ImGui::Button("Generate the World")) {
            selected = 1;
            clicked = true;
        }
        if (ImGui::Button("Options")) {
            show_options = true;
        }
        if (ImGui::Button("Quit")) {
            selected = 2;
            clicked = true;
        }
        ImGui::End();
    }

    if (show_options) {
        ImGui::SetNextWindowPosCenter();
        ImGui::Begin(win_options.c_str(), nullptr, ImVec2{600, 400}, 0.7f,
                     ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);
        ImGui::Text("Options won't take effect until you restart.");
        ImGui::Text("Full Screen Mode");
        ImGui::SameLine();
        ImGui::Checkbox("## Full Screen", &game_config.fullscreen);
        ImGui::Text("Window Height");
        ImGui::SameLine();
        ImGui::InputInt("## Window Mode Width", &game_config.window_width);
        ImGui::SameLine();
        ImGui::Text("Width");
        ImGui::SameLine();
        ImGui::InputInt(" ## Window Mode Height", &game_config.window_height);
        ImGui::Text("Autosave Every X Minutes (0=never)");
        ImGui::SameLine();
        ImGui::InputInt("## Auto save every X minutes (0 none)", &game_config.autosave_minutes);
        ImGui::Text("Allow telemetry to phone home");
        ImGui::SameLine();
        ImGui::Checkbox("## Allow telemetry to phone home", &game_config.allow_calling_home);
        ImGui::Text("Online Username");
        ImGui::SameLine();
        ImGui::InputText("Online Username", (char *) &online_username, 254);
        ImGui::Text("UI Scale Factor");
        ImGui::InputFloat("## Scale Factor", &game_config.scale_factor);
		ImGui::Text("Show Entity ID Numbers");
		ImGui::SameLine();
		ImGui::Checkbox("## Entity ID", &game_config.show_entity_ids);
        if (ImGui::Button(btn_save.c_str())) {
            game_config.online_username = std::string(online_username);
            game_config.save();
            show_options = false;
        }
        ImGui::SameLine();
        if (ImGui::Button(btn_close.c_str())) {
            show_options = false;
        }
        ImGui::End();
    }
}
