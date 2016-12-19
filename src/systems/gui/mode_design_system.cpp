#include "mode_design_system.hpp"
#include "gui_system.hpp"
#include "../mouse_input_system.hpp"
#include "../../main/game_globals.hpp"
#include "../../raws/raws.hpp"
#include "../../messages/messages.hpp"
#include "../mining_system.hpp"
#include "../inventory_system.hpp"
#include "../camera_system.hpp"
#include "../../utils/string_utils.hpp"
#include <sstream>
#include <iomanip>
#include <map>

using namespace rltk;
using namespace rltk::colors;

std::vector<available_building_t> available_buildings;
const color_t GREEN_BG{0,32,0};

void mode_design_system::configure() {
    system_name = "Design Mode";
    subscribe<refresh_available_buildings_message>([this] (refresh_available_buildings_message &msg) {
		available_buildings = get_available_buildings();
	});
}

inline bool is_mining_designation_valid(const int &x, const int &y, const int &z, const game_mining_mode_t &mode) {
	return true;
}

void mode_design_system::digging() {
    int tt_x = term(3)->term_width - 21;
    add_gui_element(std::make_unique<map_static_text>(5,4, "Digging mode - select digging type from the right panel, click to apply."));

    if (is_window_focused()) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) game_mining_mode = DIG;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) game_mining_mode = CHANNEL;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) game_mining_mode = RAMP;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) game_mining_mode = UP;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) game_mining_mode = DOWN;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) game_mining_mode = UPDOWN;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) game_mining_mode = DELETE;
    }

    if (game_mining_mode == DIG) {
        term(3)->print(tt_x+1, 7, "(d) Dig", YELLOW, DARKEST_GREEN);
    } else {
        if (mouse::term1y == 7 && mouse::term1x > tt_x+1 && mouse::term1x < tt_x+8) {
            term(3)->print(tt_x+1,7, "(d) Dig", GREEN, GREEN_BG);
            if (mouse::clicked) {
                game_mining_mode = DIG;
                return;
            }
        } else {
            term(3)->print(tt_x+1,7, "(d) Dig", WHITE, GREEN_BG);
        } 
    }
    term(3)->print(tt_x+2, 7, "d", YELLOW); 

    if (game_mining_mode == CHANNEL) { 
        term(3)->print(tt_x+1, 8, "(c) Channel", YELLOW, DARKEST_GREEN);			
    } else { 
        if (mouse::term1y == 8 && mouse::term1x > tt_x+1 && mouse::term1x < tt_x+8) {
            term(3)->print(tt_x+1,8, "(c) Channel", GREEN, GREEN_BG);
            if (mouse::clicked) {
                game_mining_mode = CHANNEL;
                return;
            }
        } else {
            term(3)->print(tt_x+1,8, "(c) Channel", WHITE, GREEN_BG);
        } 
    }
    term(3)->print(tt_x+2, 8, "c", YELLOW); 

    if (game_mining_mode == RAMP) { 
        term(3)->print(tt_x+1, 9, "(r) Ramp", YELLOW, DARKEST_GREEN); 
    } else {
        if (mouse::term1y == 9 && mouse::term1x > tt_x+1 && mouse::term1x < tt_x+8) {
            term(3)->print(tt_x+1,9, "(r) Ramp", GREEN, GREEN_BG);
            if (mouse::clicked) {
                game_mining_mode = RAMP;
                return;
            }
        } else {
            term(3)->print(tt_x+1,9, "(r) Ramp", WHITE, GREEN_BG);
        } 
    }
    term(3)->print(tt_x+2, 9, "r", YELLOW); 

    if (game_mining_mode == UP) { 
        term(3)->print(tt_x+1, 10, "(u) Up Stairs", YELLOW, DARKEST_GREEN); 
    } else { 
        if (mouse::term1y == 10 && mouse::term1x > tt_x+1 && mouse::term1x < tt_x+8) {
            term(3)->print(tt_x+1,10, "(u) Up Stairs", GREEN, GREEN_BG);
            if (mouse::clicked) {
                game_mining_mode = UP;
                return;
            }
        } else {
            term(3)->print(tt_x+1,10, "(u) Up Stairs", WHITE, GREEN_BG);
        } 		
    }
    term(3)->print(tt_x+2, 10, "u", YELLOW); 

    if (game_mining_mode == DOWN) { 
        term(3)->print(tt_x+1, 11, "(j) Down Stairs", YELLOW, DARKEST_GREEN); 
    } else { 
        if (mouse::term1y == 11 && mouse::term1x > tt_x+1 && mouse::term1x < tt_x+8) {
            term(3)->print(tt_x+1,11, "(j) Down Stairs", GREEN, GREEN_BG);
            if (mouse::clicked) {
                game_mining_mode = DOWN;
                return;
            }
        } else {
            term(3)->print(tt_x+1,11, "(j) Down Stairs", WHITE, GREEN_BG);
        } 
    }
    term(3)->print(tt_x+2, 11, "j", YELLOW); 

    if (game_mining_mode == UPDOWN) { 
        term(3)->print(tt_x+1, 12, "(i) Up/Down Stairs", YELLOW, DARKEST_GREEN); 
    } else { 
        if (mouse::term1y == 12 && mouse::term1x > tt_x+1 && mouse::term1x < tt_x+8) {
            term(3)->print(tt_x+1,12, "(i) Up/Down Stairs", GREEN, GREEN_BG);
            if (mouse::clicked) {
                game_mining_mode = UPDOWN;
                return;
            }
        } else {
            term(3)->print(tt_x+1,12, "(i) Up/Down Stairs", WHITE, GREEN_BG);
        } 
    }
    term(3)->print(tt_x+2, 12, "i", YELLOW); 

    if (game_mining_mode == DELETE) { 
        term(3)->print(tt_x+1 ,13, "(x) Clear", YELLOW, DARKEST_GREEN); 
    } else { 
        if (mouse::term1y == 13 && mouse::term1x > tt_x+1 && mouse::term1x < tt_x+8) {
            term(3)->print(tt_x+1,13, "(x) Clear", WHITE, GREEN_BG);
            if (mouse::clicked) {
                game_mining_mode = DELETE;
                return;
            }
        } else {
            term(3)->print(tt_x+1,13, "(x) Clear", WHITE, GREEN_BG);
        } 
    }
    term(3)->print(tt_x+2, 13, "x", YELLOW); 

    if (mouse::term1x >= 0 && mouse::term1x < term(1)->term_width && mouse::term1y >= 3 && mouse::term1y < term(1)->term_height) {
        if (get_mouse_button_state(rltk::button::LEFT)) {
            const int world_x = std::min(clip_left + mouse::term1x, REGION_WIDTH);
            const int world_y = std::min(clip_top + mouse::term1y-2, REGION_HEIGHT);
            const auto idx = mapidx(world_x, world_y, camera_position->region_z);
            if (is_mining_designation_valid(world_x, world_y, camera_position->region_z, game_mining_mode)) {
                switch (game_mining_mode) {
                    case DIG : designations->mining[idx] = 1; break;
                    case CHANNEL : designations->mining[idx] = 2; break;
                    case RAMP : designations->mining[idx] = 3; break;
                    case UP : designations->mining[idx] = 4; break;
                    case DOWN : designations->mining[idx] = 5; break;
                    case UPDOWN : designations->mining[idx] = 6; break;
                    case DELETE : designations->mining.erase(idx); break;
                }
                emit(map_dirty_message{});
            }
        }
    }
}

void mode_design_system::building() {
    add_gui_element(std::make_unique<map_static_text>(5,4, "Building mode - select building type from the right panel, click to apply."));

    int tt_x = term(3)->term_width - 30;
    term(3)->box(tt_x, 4, 29, term(3)->term_height-5);
    term(3)->fill(tt_x+1, 5, tt_x+29, term(3)->term_height-2, ' ');
    term(3)->fill(tt_x+1, 5, tt_x+29, 6, ' ', WHITE, DARK_GREEN);
    term(3)->print(tt_x+4, 5, "[Available Buildings]", WHITE, DARK_GREEN);

    int y=7;
    bool rendered_selected = false;
    for (const available_building_t &building : available_buildings) {
        if (build_mode_building && build_mode_building.get().tag == building.tag) {
            term(3)->print(tt_x+1, y, building.get_name(), YELLOW, DARKEST_GREEN);
            rendered_selected = true;
        } else {
            if (mouse::term3y == y && mouse::term3x > tt_x && mouse::term3x) {
                term(3)->print(tt_x+1, y, building.get_name(), GREEN, GREEN_BG);
            } else {
                term(3)->print(tt_x+1, y, building.get_name(), WHITE, GREEN_BG);
            }
        }
        ++y;
    }

    if (get_mouse_button_state(rltk::button::LEFT)) {
        
        if (mouse::term3y > 6 && mouse::term3y < 7+available_buildings.size() && mouse::term3x > tt_x+1) {
            const int selected_building = mouse::term3y - 7;
            if (selected_building > -1 && selected_building < available_buildings.size()) {
                build_mode_building = available_buildings[selected_building];
                rendered_selected = true;
            } else {
                std::cout << "Error: index " << selected_building << " is out of range.\n";
            }
        }
    }
    if (!rendered_selected) build_mode_building.reset();
}

void mode_design_system::chopping() {
    add_gui_element(std::make_unique<map_static_text>(5,4, "Tree cutting mode - click a tree to cut, right-click to clear designation."));

    if (mouse::term1x >= 0 && mouse::term1x < term(1)->term_width && mouse::term1y >= 3 && mouse::term1y < term(1)->term_height) {
        const int world_x = std::min(clip_left + mouse::term1x, REGION_WIDTH);
        const int world_y = std::min(clip_top + mouse::term1y-2, REGION_HEIGHT);

        const auto idx = mapidx(world_x, world_y, camera_position->region_z);
        const auto tree_id = current_region->tree_id[idx];

        if (get_mouse_button_state(rltk::button::LEFT) && tree_id > 0) {
            // Naieve search for the base of the tree; this could be optimized a LOT
            int lowest_z = camera_position->region_z;
            const int stop_z = lowest_z-10;

            position_t tree_pos{world_x, world_y, lowest_z};
            while (lowest_z > stop_z) {
                for (int y=-10; y<10; ++y) {
                    for (int x=-10; x<10; ++x) {
                        const int tree_idx = mapidx(world_x + x, world_y + y, lowest_z);
                        if (current_region->tree_id[tree_idx] == tree_id) {
                            tree_pos.x = world_x+x;
                            tree_pos.y = world_y+y;
                            tree_pos.z = lowest_z;
                        }
                    }
                }
                --lowest_z;
            }

            designations->chopping[(int)tree_id] = tree_pos;
            emit(map_dirty_message{});
        } else if (get_mouse_button_state(rltk::button::RIGHT) && tree_id > 0) {
            designations->chopping.erase((int)tree_id);
            emit(map_dirty_message{});
        }
    }			
}

void mode_design_system::guardposts() {
    add_gui_element(std::make_unique<map_static_text>(5,4, "Guard mode - click a square to guard it, right-click to clear."));

    if (mouse::term1x >= 0 && mouse::term1x < term(1)->term_width && mouse::term1y >= 3 && mouse::term1y < term(1)->term_height) {
        const int world_x = std::min(clip_left + mouse::term1x, REGION_WIDTH);
        const int world_y = std::min(clip_top + mouse::term1y-2, REGION_HEIGHT);

        const auto idx = mapidx(world_x, world_y, camera_position->region_z);
        if (current_region->tile_flags[idx].test(CAN_STAND_HERE)) {
            if (get_mouse_button_state(rltk::button::LEFT)) {
                bool found = false;
                for (const auto &g : designations->guard_points) {
                    if (mapidx(g.second) == idx) found = true;
                }
                if (!found) designations->guard_points.push_back(std::make_pair(false, position_t{world_x, world_y, camera_position->region_z}));
            } else if (get_mouse_button_state(rltk::button::RIGHT)) {
                designations->guard_points.erase(std::remove_if(
                        designations->guard_points.begin(),
                        designations->guard_points.end(),
                        [&idx] (std::pair<bool,position_t> p) { return idx == mapidx(p.second); }
                    ),
                    designations->guard_points.end());
            }
        }
    }
}

void mode_design_system::update(const double duration_ms) {
    if (game_master_mode != DESIGN) return;

    add_gui_element(std::make_unique<map_static_text>( 32, 1, "ESC", YELLOW));
    add_gui_element(std::make_unique<map_static_text>( 36, 1, "Resume normal play", WHITE));

    add_gui_element<gui_menu_bar>(std::vector<std::string>{"Digging", "Building", "Tree Cutting", "Guard Posts"}, 5, 3, [] (int key) {
        switch (key) {
            case 0 : { game_design_mode = DIGGING; emit_deferred(map_dirty_message{}); } break;
            case 1 : { game_design_mode = BUILDING; emit_deferred(refresh_available_buildings_message{}); emit_deferred(map_dirty_message{}); } break;
            case 2 : { game_design_mode = CHOPPING; emit_deferred(map_dirty_message{}); } break;
            case 3 : { game_design_mode = GUARDPOINTS; emit_deferred(map_dirty_message{}); } break;
        }
    });

    switch (game_design_mode) {
        case DIGGING    : digging(); break;
        case BUILDING   : building(); break;
        case CHOPPING   : chopping(); break;
        case GUARDPOINTS: guardposts(); break;
    }
}
