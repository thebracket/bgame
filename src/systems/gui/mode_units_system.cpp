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
#include "../../external/imgui-sfml/imgui-SFML.h"

using namespace rltk;
using namespace rltk::colors;

void mode_units_system::configure() {
    system_name = "Units Render System";
    subscribe_mbox<key_pressed_t>();
}

void mode_units_system::update(const double ms) {
    if (game_master_mode != UNITS) return;

    render_settlers();
    render_creatures();
    render_natives();
}

void mode_units_system::render_settlers() {
    // Build settler data structures
    std::vector<std::pair<std::size_t, std::string>> settlers;
    each<settler_ai_t, name_t, game_stats_t>([&settlers] (entity_t &e, settler_ai_t &ai, name_t &name, game_stats_t &stats) {
        settlers.emplace_back(std::make_pair(e.id, name.first_name + std::string(" ") + name.last_name + std::string(" (") + stats.profession_tag + std::string(")") ));
    });
    const char* settler_listbox_items[settlers.size()];
    for (int i=0; i<settlers.size(); ++i) {
        settler_listbox_items[i] = settlers[i].second.c_str();
    }

    // Render the settlers window
    ImGui::Begin("Settlers");
    ImGui::ListBox("", &current_settler, settler_listbox_items, settlers.size(), 10);
    if (ImGui::Button("View")) {
        game_master_mode = SETTLER;
        selected_settler = settlers[current_settler].first;
        emit_deferred(map_dirty_message{});
        emit_deferred(recalculate_mining_message{});
    }
    ImGui::SameLine();
    if (ImGui::Button("Go To")) {
        selected_settler = settlers[current_settler].first;
        auto the_settler = entity(selected_settler);
        if (the_settler) {
            auto pos = the_settler->component<position_t>();
            camera_position->region_x = pos->x;
            camera_position->region_y = pos->y;
            camera_position->region_z = pos->z;
        }
        game_master_mode = PLAY;
        emit_deferred(map_dirty_message{});
        emit_deferred(recalculate_mining_message{});
    }
    ImGui::SameLine();
    if (ImGui::Button("Control (Rogue Mode)")) {
        selected_settler = settlers[current_settler].first;
        auto the_settler = entity(selected_settler);
        if (the_settler) {
            auto pos = the_settler->component<position_t>();
            if (pos) {
                camera_position->region_x = pos->x;
                camera_position->region_y = pos->y;
                camera_position->region_z = pos->z;
            }

            auto ai = the_settler->component<settler_ai_t>();
            if (ai) {
                ai->job_type_major = JOB_IDLE;
            }
        }
        game_master_mode = ROGUE;
    }
    ImGui::SameLine();
    if (ImGui::Button("Close")) {
        game_master_mode = PLAY;
        emit_deferred(map_dirty_message{});
        emit_deferred(recalculate_mining_message{});
    }
    ImGui::End();
}

void mode_units_system::render_creatures() {
    std::vector<std::pair<std::size_t, std::string>> critters;
    each<grazer_ai, name_t>([&critters] (entity_t &e, grazer_ai &ai, name_t &name) {
        critters.emplace_back(std::make_pair(e.id, name.first_name + std::string(" ") + name.last_name));
    });
    const char* creature_listbox_items[critters.size()];
    for (int i=0; i<critters.size(); ++i) {
        creature_listbox_items[i] = critters[i].second.c_str();
    }
    ImGui::Begin("Wildlife");
    ImGui::ListBox("", &current_critter, creature_listbox_items, critters.size(), 10);
    if (ImGui::Button("Go To")) {
        auto selected_critter = critters[current_settler].first;
        auto the_critter = entity(selected_critter);
        if (the_critter) {
            auto pos = the_critter->component<position_t>();
            camera_position->region_x = pos->x;
            camera_position->region_y = pos->y;
            camera_position->region_z = pos->z;
        }
        game_master_mode = PLAY;
        emit_deferred(map_dirty_message{});
        emit_deferred(recalculate_mining_message{});
    }
    ImGui::SameLine();
    if (ImGui::Button("Close")) {
        game_master_mode = PLAY;
        emit_deferred(map_dirty_message{});
        emit_deferred(recalculate_mining_message{});
    }
    ImGui::End();
}

void mode_units_system::render_natives() {
    std::vector<std::pair<std::size_t, std::string>> natives;
    each<sentient_ai, name_t>([&natives] (entity_t &e, sentient_ai &ai, name_t &name) {
        natives.emplace_back(std::make_pair(e.id, name.first_name + std::string(" ") + name.last_name));
    });
    const char* native_listbox_items[natives.size()];
    for (int i=0; i<natives.size(); ++i) {
        native_listbox_items[i] = natives[i].second.c_str();
    }
    ImGui::Begin("Natives");
    ImGui::ListBox("", &current_native, native_listbox_items, natives.size(), 10);
    if (ImGui::Button("Go To")) {
        auto selected_critter = natives[current_settler].first;
        auto the_critter = entity(selected_critter);
        if (the_critter) {
            auto pos = the_critter->component<position_t>();
            camera_position->region_x = pos->x;
            camera_position->region_y = pos->y;
            camera_position->region_z = pos->z;
        }
        game_master_mode = PLAY;
        emit_deferred(map_dirty_message{});
        emit_deferred(recalculate_mining_message{});
    }
    ImGui::SameLine();
    if (ImGui::Button("Close")) {
        game_master_mode = PLAY;
        emit_deferred(map_dirty_message{});
        emit_deferred(recalculate_mining_message{});
    }
    ImGui::End();
}