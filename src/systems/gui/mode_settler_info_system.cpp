#include "mode_settler_info_system.hpp"
#include "../../main/game_planet.hpp"
#include "../../messages/map_dirty_message.hpp"
#include "../../messages/recalculate_mining_message.hpp"
#include "../../components/health.hpp"
#include "../../components/game_stats.hpp"
#include "../../components/settler_ai.hpp"
#include "../../components/item.hpp"
#include "../../raws/life_events.hpp"
#include "../../external/imgui-sfml/imgui-SFML.h"
#include "../../main/game_selections.hpp"
#include "../../main/game_mode.hpp"
#include "../../components/name.hpp"
#include "../../components/item_carried.hpp"

void mode_settler_info_system::configure() {
    system_name = "Settler Info";
}

void mode_settler_info_system::update(const double ms) {
    if (game_master_mode != SETTLER) return;

    auto name = entity(selected_settler)->component<name_t>();
	auto stats = entity(selected_settler)->component<game_stats_t>();
	auto species = entity(selected_settler)->component<species_t>();
	auto ai = entity(selected_settler)->component<settler_ai_t>();
	auto health = entity(selected_settler)->component<health_t>();

	std::stringstream header;
	header << " " << name->first_name << " " << name->last_name << " (" << stats->profession_tag << ") ";
    std::stringstream header2;
	header2 << species->gender_str() << ", " << species->sexuality_str() << ", " << stats->age << " years old. "
		<< species->height_feet() << ", " << species->weight_lbs();
    const std::string hitpoints = "Hit Points: " + std::to_string(health->current_hitpoints) + std::string("/") + std::to_string(health->max_hitpoints);
    std::stringstream profile;
    profile << species->gender_pronoun() << std::string(" ") << stats->strength_str() << ". ";
    profile << species->gender_pronoun() << std::string(" ") << stats->dexterity_str() << ". ";
    profile << species->gender_pronoun() << std::string(" ") << stats->constitution_str() << ". ";
    profile << species->gender_pronoun() << std::string(" ") << stats->intelligence_str() << ". ";
    profile << species->gender_pronoun() << std::string(" ") << stats->wisdom_str() << ". ";
    profile << species->gender_pronoun() << std::string(" ") << stats->charisma_str() << ". ";
    profile << species->gender_pronoun() << std::string(" ") << stats->comeliness_str() << ". ";
    profile << species->gender_pronoun() << std::string(" ") << stats->ethics_str() << ". ";
    profile << species->gender_pronoun() << std::string(" has ") << species->ethnicity() << std::string(" colored skin. ");
    profile << species->gender_pronoun() << std::string(" has ") << species->hair_color_str() << std::string(" hair, ") << species->hair_style_str() << ".";

    ImGui::Begin(header.str().c_str());
    ImGui::Text("%s", header2.str().c_str());
    ImGui::Text("%s", ai->job_status.c_str());
    ImGui::TextColored(ImVec4{1.0f, 0.0f, 0.0f, 1.0f}, "%s", hitpoints.c_str());
    for (const health_part_t &part : health->parts) {
        if (part.current_hitpoints != part.max_hitpoints) {
            std::string part_state = "OK";
            if (part.current_hitpoints < 1 && part.current_hitpoints > -10) part_state = "IMPAIRED";
            if (part.current_hitpoints < -9 && part.current_hitpoints > -20) part_state = "BROKEN";
            if (part.current_hitpoints < -19) part_state = "GONE";
            const std::string part_info = part.part + std::string(": ") + std::to_string(part.current_hitpoints) + std::string("/") + std::to_string(part.max_hitpoints) + std::string(" (") + part_state + std::string(")");
            ImGui::Text("%s", part_info.c_str());
        }
    }
    ImGui::TextWrapped("%s", profile.str().c_str());
    ImGui::TextColored(ImVec4{0.0f, 1.0f, 0.0f, 1.0f}, "Skills:");
    for (const auto &skill : stats->skills) {
        const std::string skill_display = skill.first + std::string(" : ") + std::to_string(skill.second.skill_level);
        ImGui::Text("%s", skill_display.c_str());
    }
    ImGui::TextColored(ImVec4{0.0f, 1.0f, 0.0f, 1.0f}, "Inventory:");
    each_if<item_t, item_carried_t>(
            [] (entity_t &e, item_t &item, item_carried_t &carried) {
                return carried.carried_by == selected_settler;
            },
            [] (entity_t &e, item_t &item, item_carried_t &carried) {
            const std::string item_info = item.item_name + std::string(" (") + item_loc_name(carried.location) + std::string(")");
            ImGui::Text("%s", item_info.c_str());
    });
    ImGui::TextColored(ImVec4{0.0f, 1.0f, 0.0f, 1.0f}, "History:");
    for (const life_event_t &le : planet.history.settler_life_events[selected_settler]) {
        auto finder = life_event_defs.find(le.type);
        if (finder != life_event_defs.end()) {
            const std::string line = std::to_string(le.year) + std::string(" : ") + finder->second.description;
            ImGui::Text("%s", line.c_str());
        } else {
            std::cout << "Warning: " << le.type << " life event not found\n";
        }
    }
    if (ImGui::Button("Close")) {
        game_master_mode = PLAY;
        emit_deferred(map_dirty_message{});
        emit_deferred(recalculate_mining_message{});
    }
    ImGui::End();
}
