#include "mode_units_system.hpp"
#include "../main/game_globals.hpp"
#include "../messages/map_dirty_message.hpp"
#include "../messages/recalculate_mining_message.hpp"
#include "../utils/string_utils.hpp"
#include "gui_system.hpp"
#include <rltk.hpp>

using namespace rltk;
using namespace rltk::colors;

void mode_units_system::configure() {
    system_name = "Units Render System";
    subscribe_mbox<key_pressed_t>();
}

void mode_units_system::update(const double ms) {
    if (game_master_mode != UNITS) return;

    const int term_w = rltk::term(3)->term_width;
    const int term_h = rltk::term(3)->term_height;

    const int box_left = 3;
    const int box_right = term_w - 3;
    const int box_top = 2;
    const int box_bottom = term_h - 2;

    std::unique_ptr<gui_dialog> dialog = std::make_unique<gui_dialog>(" Active Units ", [] () { 
        // On close
        game_master_mode = PLAY;
        emit_deferred(map_dirty_message{});
        emit_deferred(recalculate_mining_message{});
    });

    // Tabs system
    dialog->children.push_back(std::move(std::make_unique<gui_tab_set>(
        box_left+1, box_top+2, std::vector<std::pair<std::string, std::function<void()>>>{ 
            {"Settlers", [this] () { tab = 0; }},  
            {"Creatures", [this] () { tab = 1; }},  
            {"Natives", [this] () { tab = 2; }},  
        }, tab
    )));

    if (tab == 0) {
        // Render settler list
        std::unique_ptr<gui_table> table = std::make_unique<gui_table>(box_left+1, box_top+6);
        each<settler_ai_t, name_t, game_stats_t>([&table] (entity_t &e, settler_ai_t &ai, name_t &name, game_stats_t &stats) {
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
            std::unique_ptr<gui_table_text> c_status = std::make_unique<gui_table_text>(15, ai.job_status);
            row.cols.push_back(std::move(c_status));

            table->rows.push_back(std::move(row));
        });
        dialog->children.push_back(std::move(table));
    } else if (tab == 1) {
        // Render creature list
        std::unique_ptr<gui_table> table = std::make_unique<gui_table>(box_left+1, box_top+6);

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
        std::unique_ptr<gui_table> table = std::make_unique<gui_table>(box_left+1, box_top+6);

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

    /*
    int font_width, font_height;
    std::tie(font_width, font_height) = term(1)->get_font_size();
    const int term_w = term(1)->term_width;
    const int term_h = term(1)->term_height;

    int mouse_x, mouse_y;
	std::tie(mouse_x, mouse_y) = get_mouse_position();

    const int terminal_x = mouse_x / font_width;
    const int terminal_y = mouse_y / font_height;

    const int box_left = 3;
    const int box_right = term_w - 3;
    const int box_top = 2;
    const int box_bottom = term_h - 2;

    // Panel and header
    term(1)->fill(box_left, box_top, box_right, box_bottom, ' ');
    term(1)->box(box_left, box_top, box_right-box_left, box_bottom-box_top-1, DARK_GREEN, BLACK, true);
    term(1)->fill(box_left+1, box_top+1, box_right, box_top+2, ' ', WHITE, DARK_GREEN);
    term(1)->print_center(box_top+1, " [Active Units] ", WHITE, GREEN);

    // Tabs

    if (get_mouse_button_state(rltk::button::LEFT) && terminal_x > box_left && 
        terminal_x < box_left + 10 && terminal_y > box_top+2 && terminal_y < box_top+4) 
    {
        tab = 0;
    }
    if (get_mouse_button_state(rltk::button::LEFT) && terminal_x > box_left+11 && 
        terminal_x < box_left + 27 && terminal_y > box_top+2 && terminal_y < box_top+4) 
    {
        tab = 1;
    }
    if (get_mouse_button_state(rltk::button::LEFT) && terminal_x > box_left+23 && 
        terminal_x < box_left + 33 && terminal_y > box_top+2 && terminal_y < box_top+4) 
    {
        tab = 2;
    }

    if (tab == 0) {
        
        term(1)->box(box_left+1, box_top+2, 9, 2, GREEN, BLACK, false);
        term(1)->fill(box_left+2, box_top+3, box_left+11, box_top+3, ' ', DARK_GREEN, BLACK);
        term(1)->print(box_left+2, box_top+3, "Settlers", WHITE, BLACK);

        int y = box_top + 6;
        each<settler_ai_t, name_t, game_stats_t>([this, &y, &terminal_x, &terminal_y, &box_left] (entity_t &e, settler_ai_t &ai, name_t &name, game_stats_t &stats) {
            color_t background = BLACK;

            if (terminal_y == y && terminal_x > 21) {
                background = DARK_GREEN;
                if (get_mouse_button_state(rltk::button::LEFT)) {
                    selected_settler = e.id;
                    game_master_mode = SETTLER;
                }
            }

            if (terminal_y == y && terminal_x > box_left+3 && terminal_x < box_left+8) {
                term(1)->print(box_left+3, y, " GO ", WHITE, DARK_GREEN);
                if (get_mouse_button_state(rltk::button::LEFT)) {
                    // Move the camera and close
                    game_master_mode = PLAY;
                    emit(map_dirty_message{});
                    emit(recalculate_mining_message{});
                    auto pos = e.component<position_t>();
                    camera_position->region_x = pos->x;
                    camera_position->region_y = pos->y;
                    camera_position->region_z = pos->z;
                }
            } else {
                term(1)->print(box_left+3, y, " GO ", WHITE, DARKEST_GREEN);
            }
            if (terminal_y == y && terminal_x > box_left+8 && terminal_x < box_left+19) {
                term(1)->print(box_left+8, y, " CONTROL ", WHITE, DARK_GREEN);
                if (get_mouse_button_state(rltk::button::LEFT)) {
                    // Switch to rogue mode and close
                    game_master_mode = ROGUE;
                    selected_settler = e.id;
                    emit(map_dirty_message{});
                    emit(recalculate_mining_message{});
                    auto pos = e.component<position_t>();
                    camera_position->region_x = pos->x;
                    camera_position->region_y = pos->y;
                    camera_position->region_z = pos->z;
                    entity(e.id)->component<settler_ai_t>()->job_type_major = JOB_IDLE;
                }
            } else {
                term(1)->print(box_left+8, y, " CONTROL ", WHITE, DARKEST_GREEN);
            }
            
            term(1)->print(22, y, max_width_str(name.first_name + std::string(" ") + name.last_name, 20), WHITE, background);
            term(1)->print(42, y, max_width_str(stats.profession_tag, 14), WHITE, background);
            term(1)->print(57, y, max_width_str(ai.job_status, 29), WHITE, background);

            ++y;
        });
    } else {
        term(1)->box(box_left+1, box_top+2, 9, 2, DARK_GREEN, BLACK, false);
        term(1)->fill(box_left+2, box_top+3, box_left+11, box_top+3, ' ', BLACK, BLACK);
        term(1)->print(box_left+2, box_top+3, "Settlers", DARK_GREEN, BLACK);
    }

    if (tab == 1) {
        term(1)->box(box_left+11, box_top+2, 10, 2, GREEN, BLACK, false);
        term(1)->fill(box_left+12, box_top+3, box_left+26, box_top+3, ' ', DARK_GREEN, BLACK);
        term(1)->print(box_left+12, box_top+3, "Creatures", WHITE, BLACK);

        int y = box_top + 6;

        each<grazer_ai, name_t, species_t>([&y,&terminal_x,&terminal_y, &box_left] (entity_t &e, grazer_ai &ai, name_t &name, species_t &species) {
            color_t background = BLACK;

            if (terminal_y == y && terminal_x > 21) {
                background = DARK_GREEN;
            }

            if (terminal_y == y && terminal_x > box_left+3 && terminal_x < box_left+8) {
                term(1)->print (box_left+3, y, " GO ", WHITE, DARK_GREEN);
                if (get_mouse_button_state(rltk::button::LEFT)) {
                    // Move the camera and close
                    game_master_mode = PLAY;
                    emit(map_dirty_message{});
                    emit(recalculate_mining_message{});
                    auto pos = e.component<position_t>();
                    camera_position->region_x = pos->x;
                    camera_position->region_y = pos->y;
                    camera_position->region_z = pos->z;
                }
            } else {
                term(1)->print (box_left+3, y, " GO ", WHITE, DARKEST_GREEN);
            }
            term(1)->print(box_left+8, y, max_width_str(name.first_name + std::string(" ") + name.last_name, 20), WHITE, background);
            ++y;
        });
    } else {
        term(1)->box(box_left+11, box_top+2, 10, 2, DARK_GREEN, BLACK, false);
        term(1)->fill(box_left+12, box_top+3, box_left+26, box_top+3, ' ', BLACK, BLACK);
        term(1)->print(box_left+12, box_top+3, "Creatures", DARK_GREEN, BLACK);
    }

    if (tab == 2) {
        term(1)->box(box_left+22, box_top+2, 8, 2, DARK_GREEN, BLACK, false);
        term(1)->fill(box_left+23, box_top+3, box_left+36, box_top+3, ' ', BLACK, BLACK);
        term(1)->print(box_left+23, box_top+3, "Natives", WHITE, BLACK);

        int y = box_top + 6;

        each<sentient_ai, name_t>([&y,&terminal_x,&terminal_y, &box_left] (entity_t &e, sentient_ai &ai, name_t &name) {
            color_t background = BLACK;

            if (terminal_y == y && terminal_x > 21) {
                background = DARK_GREEN;
            }

            if (terminal_y == y && terminal_x > box_left+3 && terminal_x < box_left+8) {
                term(1)->print (box_left+3, y, " GO ", WHITE, DARK_GREEN);
                if (get_mouse_button_state(rltk::button::LEFT)) {
                    // Move the camera and close
                    game_master_mode = PLAY;
                    emit(map_dirty_message{});
                    emit(recalculate_mining_message{});
                    auto pos = e.component<position_t>();
                    camera_position->region_x = pos->x;
                    camera_position->region_y = pos->y;
                    camera_position->region_z = pos->z;
                }
            } else {
                term(1)->print (box_left+3, y, " GO ", WHITE, DARKEST_GREEN);
            }
            term(1)->print(box_left+8, y, max_width_str(name.first_name + std::string(" ") + name.last_name, 20), WHITE, background);
            ++y;
        });
    } else {
        term(1)->box(box_left+22, box_top+2, 8, 2, DARK_GREEN, BLACK, false);
        term(1)->fill(box_left+23, box_top+3, box_left+36, box_top+3, ' ', BLACK, BLACK);
        term(1)->print(box_left+23, box_top+3, "Natives", DARK_GREEN, BLACK);
    }
    */
}
