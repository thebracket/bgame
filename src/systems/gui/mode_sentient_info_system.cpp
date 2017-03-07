#include "mode_sentient_info_system.hpp"
#include "../../main/game_globals.hpp"
#include "../../raws/raws.hpp"
#include "../../raws/species.hpp"
#include "gui_system.hpp"
#include "../../messages/map_dirty_message.hpp"
#include "../../messages/recalculate_mining_message.hpp"
#include "../../components/health.hpp"
#include "../../components/species.hpp"
#include "../../components/sentient_ai.hpp"
#include "../../components/item.hpp"
#include "../../external/imgui-sfml/imgui-SFML.h"
#include "../../components/grazer_ai.hpp"
#include "../../raws/creatures.hpp"

void mode_sentient_info_system::configure() { 
    system_name = "Creature Info System";   
}

void display_sentient_info()
{
    auto name = entity(selected_settler)->component<name_t>();
	auto species = entity(selected_settler)->component<species_t>();
	auto ai = entity(selected_settler)->component<sentient_ai>();
    auto health = entity(selected_settler)->component<health_t>();
    auto species_finder = *get_species_def(species->tag);

	std::stringstream header;
	header << " " << name->first_name << " " << name->last_name;

    std::stringstream header2;
    header2 << species->gender_str() << " " << species_finder.name;

    std::stringstream header3;
    header3 << "Member of: " << planet.civs.civs[ai->civ_id].name;
    if (planet.civs.civs[ai->civ_id].cordex_feelings < 0) {
        header3 << " (HOSTILE)";        
    } else if (planet.civs.civs[ai->civ_id].cordex_feelings == 0) {
        header3 << " (NEUTRAL)";
    } else {
        header3 << " (FRIENDLY)";
    }
    header3 << ".";
    const std::string hitpoints = "Hit Points: " + std::to_string(health->current_hitpoints) + std::string("/") + std::to_string(health->max_hitpoints);

    ImGui::Begin(header.str().c_str());
    ImGui::Text(header2.str().c_str());
    ImGui::Text(species_finder.description.c_str());
    ImGui::Text(header3.str().c_str());
    ImGui::TextColored(ImVec4{1.0f, 0.0f, 0.0f, 1.0f}, hitpoints.c_str());
    for (const health_part_t &part : health->parts) {
        if (part.current_hitpoints != part.max_hitpoints) {
            std::string part_state = "OK";
            if (part.current_hitpoints < 1 && part.current_hitpoints > -10) part_state = "IMPAIRED";
            if (part.current_hitpoints < -9 && part.current_hitpoints > -20) part_state = "BROKEN";
            if (part.current_hitpoints < -19) part_state = "GONE";
            const std::string part_info = part.part + std::string(": ") + std::to_string(part.current_hitpoints) + std::string("/") + std::to_string(part.max_hitpoints) + std::string(" (") + part_state + std::string(")");
            ImGui::Text(part_info.c_str());
        }
    }
    ImGui::TextColored(ImVec4{0.0f, 1.0f, 0.0f, 1.0f}, "Inventory:");
    each<item_t, item_carried_t>([] (entity_t &e, item_t &item, item_carried_t &carried) {
        if (carried.carried_by == selected_settler) {
            const std::string item_info = item.item_name + std::string(" (") + item_loc_name(carried.location) + std::string(")");
            ImGui::Text(item_info.c_str());
        }
    });
    if (ImGui::Button("Close")) {
        game_master_mode = PLAY;
        emit_deferred(map_dirty_message{});
        emit_deferred(recalculate_mining_message{});
    }
    ImGui::End();
}

void display_grazer_info() {
    auto name = entity(selected_settler)->component<name_t>();
	auto species = entity(selected_settler)->component<species_t>();
	auto ai = entity(selected_settler)->component<sentient_ai>();
    auto health = entity(selected_settler)->component<health_t>();
    auto species_finder = *get_creature_def(species->tag);

	std::stringstream header;
	header << " " << name->first_name << " " << name->last_name;

    std::stringstream header2;
    header2 << species_finder.name;

    const std::string hitpoints = "Hit Points: " + std::to_string(health->current_hitpoints) + std::string("/") + std::to_string(health->max_hitpoints);

    ImGui::Begin(header.str().c_str());
    ImGui::Text(header2.str().c_str());
    ImGui::Text(species_finder.description.c_str());
    ImGui::TextColored(ImVec4{1.0f, 0.0f, 0.0f, 1.0f}, hitpoints.c_str());
    for (const health_part_t &part : health->parts) {
        if (part.current_hitpoints != part.max_hitpoints) {
            std::string part_state = "OK";
            if (part.current_hitpoints < 1 && part.current_hitpoints > -10) part_state = "IMPAIRED";
            if (part.current_hitpoints < -9 && part.current_hitpoints > -20) part_state = "BROKEN";
            if (part.current_hitpoints < -19) part_state = "GONE";
            const std::string part_info = part.part + std::string(": ") + std::to_string(part.current_hitpoints) + std::string("/") + std::to_string(part.max_hitpoints) + std::string(" (") + part_state + std::string(")");
            ImGui::Text(part_info.c_str());
        }
    }
    if (ImGui::Button("Close")) {
        game_master_mode = PLAY;
        emit_deferred(map_dirty_message{});
        emit_deferred(recalculate_mining_message{});
    }
    ImGui::End();
}

void mode_sentient_info_system::update(const double ms) {
    if (game_master_mode != SENTIENT_INFO && game_master_mode != GRAZER_INFO) return;

    if (game_master_mode == SENTIENT_INFO) {
        display_sentient_info();
    } else {
        display_grazer_info();
    }
}
