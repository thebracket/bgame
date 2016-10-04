#include "keyboard_system.hpp"
#include "../messages/messages.hpp"
#include "../main/game_globals.hpp"

void keyboard_system::configure() {
    system_name = "Keyboard Handler";
	subscribe_mbox<key_pressed_t>();
}

void keyboard_system::update(const double ms) {
    // Keyboard queue
    std::queue<key_pressed_t> * messages = mbox<key_pressed_t>();
    while (!messages->empty()) {
		key_pressed_t e = messages->front();

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
            } else if (e.event.key.code == sf::Keyboard::W) {
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
        }
	messages->pop();
	}
}
