#include "keyboard_system.hpp"
#include "../../messages/messages.hpp"
#include "../../main/game_globals.hpp"

bool collect_text = false;
std::string input_text;

void keyboard_system::configure() {
    system_name = "Keyboard Handler";
	subscribe_mbox<key_pressed_t>();
}

void keyboard_system::process_input(sf::Event &e) {
    char pressed=0;
    switch (e.key.code) {
        case sf::Keyboard::A : pressed = 'A'; break;
        case sf::Keyboard::B : pressed = 'B'; break;
        case sf::Keyboard::C : pressed = 'C'; break;
        case sf::Keyboard::D : pressed = 'D'; break;
        case sf::Keyboard::E : pressed = 'E'; break;
        case sf::Keyboard::F : pressed = 'F'; break;
        case sf::Keyboard::G : pressed = 'G'; break;
        case sf::Keyboard::H : pressed = 'H'; break;
        case sf::Keyboard::I : pressed = 'I'; break;
        case sf::Keyboard::J : pressed = 'J'; break;
        case sf::Keyboard::K : pressed = 'K'; break;
        case sf::Keyboard::L : pressed = 'L'; break;
        case sf::Keyboard::M : pressed = 'M'; break;
        case sf::Keyboard::N : pressed = 'N'; break;
        case sf::Keyboard::O : pressed = 'O'; break;
        case sf::Keyboard::P : pressed = 'P'; break;
        case sf::Keyboard::Q : pressed = 'Q'; break;
        case sf::Keyboard::R : pressed = 'R'; break;
        case sf::Keyboard::S : pressed = 'S'; break;
        case sf::Keyboard::T : pressed = 'T'; break;
        case sf::Keyboard::U : pressed = 'U'; break;
        case sf::Keyboard::V : pressed = 'V'; break;
        case sf::Keyboard::W : pressed = 'W'; break;
        case sf::Keyboard::X : pressed = 'X'; break;
        case sf::Keyboard::Y : pressed = 'Y'; break;
        case sf::Keyboard::Z : pressed = 'Z'; break;
        case sf::Keyboard::Num0 : pressed = '0'; break;
        case sf::Keyboard::Num1 : pressed = '1'; break;
        case sf::Keyboard::Num2 : pressed = '2'; break;
        case sf::Keyboard::Num3 : pressed = '3'; break;
        case sf::Keyboard::Num4 : pressed = '4'; break;
        case sf::Keyboard::Num5 : pressed = '5'; break;
        case sf::Keyboard::Num6 : pressed = '6'; break;
        case sf::Keyboard::Num7 : pressed = '7'; break;
        case sf::Keyboard::Num8 : pressed = '8'; break;
        case sf::Keyboard::Num9 : pressed = '9'; break;
        case sf::Keyboard::Space : pressed = ' '; break;
        case sf::Keyboard::BackSpace : pressed = 1; break;
        case sf::Keyboard::Return : pressed = 2; break;
    }

    if (pressed == 1 && !input_text.empty()) {
        input_text = input_text.substr(0, input_text.size()-1);
    }

    if (pressed>='A' && pressed <='Z' && !e.key.shift) pressed += 32;

    if (pressed == 2) collect_text = false;

    if (pressed > 2) input_text += pressed;
}

void keyboard_system::update(const double ms) {
    // Keyboard queue
    std::queue<key_pressed_t> * messages = mbox<key_pressed_t>();
    while (!messages->empty()) {
		key_pressed_t e = messages->front();

        if (collect_text) process_input(e.event);
        
        // Global commands
        if (e.event.key.code == sf::Keyboard::Q) quitting = true;
        if (e.event.key.code == sf::Keyboard::F1) std::cout << ecs_profile_dump() << "\n";

        // Pause control
        if (e.event.key.code == sf::Keyboard::Space) emit_deferred(pause_requested_message{});
        if (e.event.key.code == sf::Keyboard::Slash) emit_deferred(one_step_requested_message{});

        // Camera controls
        if (e.event.key.code == sf::Keyboard::Left) {
            if (e.event.key.shift) { 
                emit_deferred(camera_move_requested_message{1, 5}); 
            } else { 
                emit_deferred(camera_move_requested_message{1, 1}); 
            }
        }
        if (e.event.key.code == sf::Keyboard::Right) {
            if (e.event.key.shift) { 
                emit_deferred(camera_move_requested_message{2, 5}); 
            } else { 
                emit_deferred(camera_move_requested_message{2, 1}); 
            }
        }
        if (e.event.key.code == sf::Keyboard::Up) {
            if (e.event.key.shift) { 
                emit_deferred(camera_move_requested_message{3, 5}); 
            } else { 
                emit_deferred(camera_move_requested_message{3, 1}); 
            }
        }
        if (e.event.key.code == sf::Keyboard::Down) {
            if (e.event.key.shift) { 
                emit_deferred(camera_move_requested_message{4, 5}); 
            } else { 
                emit_deferred(camera_move_requested_message{4, 1}); 
            }
        }
        if (e.event.key.code == sf::Keyboard::Period) {
            emit_deferred(camera_move_requested_message{5, 1}); 
        }
        if (e.event.key.code == sf::Keyboard::Comma) {
            emit_deferred(camera_move_requested_message{6, 1}); 
        }

        // Mode changes
        if (game_master_mode == PLAY) {
            if (e.event.key.code == sf::Keyboard::D) {
                game_master_mode = DESIGN;
                game_design_mode = DIGGING;
                pause_mode = PAUSED;
                emit_deferred(map_dirty_message{});
            } else if (e.event.key.code == sf::Keyboard::U) {
                game_master_mode = UNITS;
                pause_mode = PAUSED;
                emit_deferred(map_dirty_message{});
            } else if (e.event.key.code == sf::Keyboard::W && !e.event.key.control) {
                game_master_mode = WORKFLOW;
                pause_mode = PAUSED;
                emit_deferred(map_dirty_message{});
            } else if (e.event.key.code == sf::Keyboard::C) {
                game_master_mode = CIVS;
                pause_mode = PAUSED;
                emit_deferred(map_dirty_message{});
            } else if (e.event.key.code == sf::Keyboard::S) {
                game_master_mode = STANDING_ORDERS;
                pause_mode = PAUSED;
                emit_deferred(map_dirty_message{});
            } else if (e.event.key.code == sf::Keyboard::W && e.event.key.control) {
                game_master_mode = WISHMODE;
                pause_mode = PAUSED;
                emit_deferred(map_dirty_message{});
                collect_text = true;
                input_text = "";
            }
        } else if (game_master_mode == DESIGN) {
            if (e.event.key.code == sf::Keyboard::Escape) {
                game_master_mode = PLAY;
                emit_deferred(map_dirty_message{});
                emit_deferred(recalculate_mining_message{});
            }
            if (e.event.key.code == sf::Keyboard::D) {
                game_design_mode = DIGGING;
                emit_deferred(map_dirty_message{});
            }
            if (e.event.key.code == sf::Keyboard::B) {
                game_design_mode = BUILDING;
                emit_deferred(refresh_available_buildings_message{});
                emit_deferred(map_dirty_message{});
            }
            if (e.event.key.code == sf::Keyboard::T) {
                game_design_mode = CHOPPING;
                emit_deferred(map_dirty_message{});
            }
            if (e.event.key.code == sf::Keyboard::G) {
                game_design_mode = GUARDPOINTS;
                emit_deferred(map_dirty_message{});
            }
        } else if (game_master_mode == SETTLER) {
            if (e.event.key.code == sf::Keyboard::Escape) {
                game_master_mode = PLAY;
                emit_deferred(map_dirty_message{});
                emit_deferred(recalculate_mining_message{});
            }
        } else if (game_master_mode == WORKFLOW) {
            if (e.event.key.code == sf::Keyboard::Escape) {
                game_master_mode = PLAY;
                emit_deferred(map_dirty_message{});
                emit_deferred(recalculate_mining_message{});
            }
        } else if (game_master_mode == ROGUE) {
            if (e.event.key.code == sf::Keyboard::Escape) {
                game_master_mode = PLAY;
                emit_deferred(map_dirty_message{});
                emit_deferred(recalculate_mining_message{});
            }
        } else if (game_master_mode == UNITS) {
            if (e.event.key.code == sf::Keyboard::Escape) {
                game_master_mode = PLAY;
                emit_deferred(map_dirty_message{});
                emit_deferred(recalculate_mining_message{});
            }
        } else if (game_master_mode == CIVS) {
            if (e.event.key.code == sf::Keyboard::Escape) {
                game_master_mode = PLAY;
                emit_deferred(map_dirty_message{});
                emit_deferred(recalculate_mining_message{});
            }
        } else if (game_master_mode == CIV_NEGOTIATE) {
            if (e.event.key.code == sf::Keyboard::Escape) {
                game_master_mode = PLAY;
                emit_deferred(map_dirty_message{});
                emit_deferred(recalculate_mining_message{});
            }
        } else if (game_master_mode == TILEMENU || game_master_mode == SENTIENT_INFO || game_master_mode == GRAZER_INFO) {
            if (e.event.key.code == sf::Keyboard::Escape) {
                game_master_mode = PLAY;
                emit_deferred(map_dirty_message{});
                emit_deferred(recalculate_mining_message{});
            }
        } else if (game_master_mode == WISHMODE) {
            if (e.event.key.code == sf::Keyboard::Escape) {
                game_master_mode = PLAY;
                emit_deferred(map_dirty_message{});
                emit_deferred(recalculate_mining_message{});
            }
        }
	messages->pop();
	}
}
