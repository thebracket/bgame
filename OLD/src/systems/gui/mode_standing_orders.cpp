#include "mode_standing_orders.hpp"
#include "../../main/game_mode.hpp"

void mode_standing_orders::configure() {
    system_name = "Standing Orders";
}

void mode_standing_orders::update(const double ms) {
    if (game_master_mode != STANDING_ORDERS) return;

    /*
    dialog_placement_t box{};

    std::unique_ptr<gui_dialog> dialog = std::make_unique<gui_dialog>(" Standing Orders ", [] () { 
        // On close
        game_master_mode = PLAY;
        emit_deferred(map_dirty_message{});
        emit_deferred(recalculate_mining_message{});
    });

    dialog->children.push_back(std::move(std::make_unique<gui_radio>(
        box.left+1, box.top + 4, "When idle, settlers will...", std::vector<std::string>{"Not move", "Wander", "Stay close to Cordex"}, designations->standing_order_idle_move,
        [] (const int new_mode) { designations->standing_order_idle_move = new_mode; }
    )));

    dialog->children.push_back(std::move(std::make_unique<gui_radio>(
        box.left+1, box.top + 9, "When encountering wildlife, settlers will...", std::vector<std::string>{"Ignore it", "Kill it"}, designations->standing_order_wildlife_treatment,
        [] (const int new_mode) { designations->standing_order_wildlife_treatment = new_mode; }
    )));

    dialog->children.push_back(std::move(std::make_unique<gui_radio>(
        box.left+1, box.top + 14, "When better equipment is available, settlers will...", std::vector<std::string>{"Not seek an upgrade", "Upgrade, but only if it is nearby", "Always seek out an upgrade"}, designations->standing_order_upgrade,
        [] (const int new_mode) { designations->standing_order_upgrade = new_mode; }
    )));


    add_gui_element(std::move(dialog));
    */
    // TODO: Rewrite for ImGui
}
