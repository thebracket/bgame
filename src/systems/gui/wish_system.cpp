#include "wish_system.hpp"
#include "../../main/game_globals.hpp"
#include "../input/keyboard_system.hpp"
#include "../../messages/messages.hpp"

using namespace rltk;

void wish_system::configure() {
    system_name = "Make A Wish";
}

void wish_system::update(const double ms) {
    if (game_master_mode != WISHMODE) return;

    term(1)->box(5, (term(1)->term_height/2)-3, term(1)->term_width-7, 4);
    term(1)->print_center((term(1)->term_height/2)-4, "Make A Wish (GM Command)", rltk::colors::YELLOW);
    term(1)->print_center((term(1)->term_height/2)-3, "Escape to return to play, otherwise type your wish and press ENTER.");
    term(1)->print_center((term(1)->term_height/2)-1, input_text, rltk::colors::GREEN);

    if (!collect_text) {
        if (input_text == "sploosh") {
            std::cout << "Let it rain!\n";
            for (int y=1; y<REGION_HEIGHT-1; ++y) {
                for (int x=1; x<REGION_WIDTH-1; ++x) {
                    current_region->water_level[mapidx(x,y,REGION_DEPTH-2)] = 3;
                }
            }
        }

        game_master_mode = PLAY;
        emit_deferred(map_dirty_message{});
        emit_deferred(recalculate_mining_message{});
    } 
}