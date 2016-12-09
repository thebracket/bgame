#include "pause_system.hpp"
#include "../main/game_globals.hpp"
#include "../messages/messages.hpp"

void pause_system::configure() {
    system_name = "Pausing";

    subscribe<pause_requested_message>([this] (pause_requested_message &msg) { requested_pause = true; });
    subscribe<one_step_requested_message>([this] (one_step_requested_message &msg) { requested_step = true; });
}

void pause_system::update(const double duration_ms) {
    if (game_master_mode == PLAY) {
        if (requested_pause) {
            if (pause_mode == RUNNING) {
                pause_mode = PAUSED;
            } else {
                pause_mode = RUNNING;
            }
        }
        if (requested_step) {
            pause_mode = ONE_STEP;
        }
    }

    requested_pause = false;
    requested_step = false;
}
