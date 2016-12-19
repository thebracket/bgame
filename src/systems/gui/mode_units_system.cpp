#include "mode_units_system.hpp"
#include "../../main/game_globals.hpp"
#include "../../messages/map_dirty_message.hpp"
#include "../../messages/recalculate_mining_message.hpp"
#include "../../utils/string_utils.hpp"
#include "gui_system.hpp"
#include "../../components/settler_ai.hpp"
#include "../../components/game_stats.hpp"
#include "../../components/sentient_ai.hpp"
#include "../../components/grazer_ai.hpp"
#include <rltk.hpp>

using namespace rltk;
using namespace rltk::colors;

void mode_units_system::configure() {
    system_name = "Units Render System";
    subscribe_mbox<key_pressed_t>();
}

void mode_units_system::update(const double ms) {
    if (game_master_mode != UNITS) return;

    dialog_placement_t box{};

    std::unique_ptr<gui_dialog> dialog = std::make_unique<gui_dialog>(" Active Units ", [] () { 
        // On close
        game_master_mode = PLAY;
        emit_deferred(map_dirty_message{});
        emit_deferred(recalculate_mining_message{});
    });

    // Tabs system
    dialog->children.push_back(std::move(std::make_unique<gui_tab_set>(
        box.left+1, box.top+2, std::vector<std::pair<std::string, std::function<void()>>>{ 
            {"Settlers", [this] () { tab = 0; }},  
            {"Creatures", [this] () { tab = 1; }},  
            {"Natives", [this] () { tab = 2; }},  
        }, tab
    )));

    if (tab == 0) {
        // Render settler list
        std::unique_ptr<gui_table> table = std::make_unique<gui_table>(box.left+1, box.top+6);
        each<settler_ai_t, name_t, game_stats_t>([&table, &box ] (entity_t &e, settler_ai_t &ai, name_t &name, game_stats_t &stats) {
            gui_table_row row;

            // Go button
            std::unique_ptr<gui_table_button> go_btn = std::make_unique<gui_table_button>(4, " GO ", [&e] () {
                game_master_mode = PLAY;
                emit(map_dirty_message{});
                emit(recalculate_mining_message{});
                auto pos = e.component<position_t>();
                camera_position->region_x = pos->x;
                camera_position->region_y = pos->y;
                camera_position->region_z = pos->z;
            });
            row.cols.push_back(std::move(go_btn));

            // Control button
            std::unique_ptr<gui_table_button> rogue_btn = std::make_unique<gui_table_button>(9, " CONTROL ", [&e] () {
                game_master_mode = ROGUE;
                selected_settler = e.id;
                emit(map_dirty_message{});
                emit(recalculate_mining_message{});
                auto pos = e.component<position_t>();
                camera_position->region_x = pos->x;
                camera_position->region_y = pos->y;
                camera_position->region_z = pos->z;
                entity(e.id)->component<settler_ai_t>()->job_type_major = JOB_IDLE;
            });
            row.cols.push_back(std::move(rogue_btn));

            // Settler name
            std::unique_ptr<gui_table_button> c_name = std::make_unique<gui_table_button>(20, name.first_name + std::string(" ") + name.last_name,
                [&e] () {
                    selected_settler = e.id;
                    game_master_mode = SETTLER;
                });
            row.cols.push_back(std::move(c_name));

            // Settler Profession
            std::unique_ptr<gui_table_text> c_prof = std::make_unique<gui_table_text>(20, stats.profession_tag);
            row.cols.push_back(std::move(c_prof));

            // Settler Status
            std::unique_ptr<gui_table_text> c_status = std::make_unique<gui_table_text>(box.right - 54, ai.job_status);
            row.cols.push_back(std::move(c_status));

            table->rows.push_back(std::move(row));
        });
        dialog->children.push_back(std::move(table));
    } else if (tab == 1) {
        // Render creature list
        std::unique_ptr<gui_table> table = std::make_unique<gui_table>(box.left+1, box.top+6);

        each<grazer_ai, name_t>([&table] (entity_t &e, grazer_ai &ai, name_t &name) {
            gui_table_row row;

            std::unique_ptr<gui_table_button> go_btn = std::make_unique<gui_table_button>(4, " GO ", [&e] () {
                game_master_mode = PLAY;
                emit(map_dirty_message{});
                auto pos = e.component<position_t>();
                camera_position->region_x = pos->x;
                camera_position->region_y = pos->y;
                camera_position->region_z = pos->z;
            });
            row.cols.push_back(std::move(go_btn));

            std::unique_ptr<gui_table_text> c_name = std::make_unique<gui_table_text>(20, name.first_name + std::string(" ") + name.last_name);
            row.cols.push_back(std::move(c_name));
            table->rows.push_back(std::move(row));
        });

        dialog->children.push_back(std::move(table));
    } else {
        // Render natives list
        std::unique_ptr<gui_table> table = std::make_unique<gui_table>(box.left+1, box.top+6);

        each<sentient_ai, name_t>([&table] (entity_t &e, sentient_ai &ai, name_t &name) {
            gui_table_row row;

            std::unique_ptr<gui_table_button> go_btn = std::make_unique<gui_table_button>(4, " GO ", [&e] () {
                game_master_mode = PLAY;
                emit(map_dirty_message{});
                emit(recalculate_mining_message{});
                auto pos = e.component<position_t>();
                camera_position->region_x = pos->x;
                camera_position->region_y = pos->y;
                camera_position->region_z = pos->z;
            });
            row.cols.push_back(std::move(go_btn));

            std::unique_ptr<gui_table_text> c_name = std::make_unique<gui_table_text>(20, name.first_name + std::string(" ") + name.last_name);
            row.cols.push_back(std::move(c_name));
            table->rows.push_back(std::move(row));
        });

        dialog->children.push_back(std::move(table));
    }

    add_gui_element(std::move(dialog));
}
