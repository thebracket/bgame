#include "mode_settler_info_system.hpp"
#include "../main/game_globals.hpp"
#include "gui_system.hpp"
#include "../messages/map_dirty_message.hpp"
#include "../messages/recalculate_mining_message.hpp"
#include "../components/health.hpp"
#include "../components/game_stats.hpp"
#include "../components/species.hpp"
#include "../components/settler_ai.hpp"
#include "../components/item.hpp"

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

    dialog_placement_t box{};

    std::unique_ptr<gui_dialog> dialog = std::make_unique<gui_dialog>(header.str(), [] () { 
        // On close
        game_master_mode = PLAY;
        emit_deferred(map_dirty_message{});
        emit_deferred(recalculate_mining_message{});
    });

    dialog->children.push_back(std::move(std::make_unique<gui_static_text>(box.left+2, box.top+2, header2.str(), rltk::colors::MAGENTA, rltk::colors::DARKEST_GREEN)));
    dialog->children.push_back(std::move(std::make_unique<gui_static_text>(box.left+2, box.top+3, ai->job_status, rltk::colors::CYAN, rltk::colors::DARKEST_GREEN)));

    // Tabs system
    dialog->children.push_back(std::move(std::make_unique<gui_tab_set>(
        box.left+1, box.top+4, std::vector<std::pair<std::string, std::function<void()>>>{ 
            {"Profile", [this] () { tab = 0; }},  
            {"Skills", [this] () { tab = 1; }},  
            {"Inventory", [this] () { tab = 2; }},  
            {"History", [this] () { tab = 3; }}
        }, tab
    )));

    if (tab == 0) {
        dialog->children.push_back(std::move(std::make_unique<gui_static_text>(box.left+30, box.top+7, "Hit Points: " + std::to_string(health->current_hitpoints) + std::string("/") + std::to_string(health->max_hitpoints), rltk::colors::RED)));

        int y = box.top+8;
        for (const health_part_t &part : health->parts) {
            if (part.current_hitpoints != part.max_hitpoints) {
                std::string part_state = "OK";
                if (part.current_hitpoints < 1 && part.current_hitpoints > -10) part_state = "IMPAIRED";
                if (part.current_hitpoints < -9 && part.current_hitpoints > -20) part_state = "BROKEN";
                if (part.current_hitpoints < -19) part_state = "GONE";
                term(1)->print(2,y, part.part + std::string(": ") + std::to_string(part.current_hitpoints) + std::string("/") + std::to_string(part.max_hitpoints) + std::string(" (") + part_state + std::string(")"));
                dialog->children.push_back(std::move(std::make_unique<gui_static_text>(box.left+30, y, part.part + std::string(": ") + std::to_string(part.current_hitpoints) + std::string("/") + std::to_string(part.max_hitpoints) + std::string(" (") + part_state + std::string(")"),      
                    rltk::colors::MAGENTA, rltk::colors::DARKEST_GREEN)));
                ++y;
            }
        }
        ++y;

        dialog->children.push_back(std::move(std::make_unique<gui_static_text>(box.left+30, y, species->gender_pronoun() + std::string(" ") + stats->strength_str(), rltk::colors::WHITE, rltk::colors::DARKEST_GREEN))); ++y;
        dialog->children.push_back(std::move(std::make_unique<gui_static_text>(box.left+30, y, species->gender_pronoun() + std::string(" ") + stats->dexterity_str(), rltk::colors::WHITE, rltk::colors::DARKEST_GREEN))); ++y;
        dialog->children.push_back(std::move(std::make_unique<gui_static_text>(box.left+30, y, species->gender_pronoun() + std::string(" ") + stats->constitution_str(), rltk::colors::WHITE, rltk::colors::DARKEST_GREEN))); ++y;
        dialog->children.push_back(std::move(std::make_unique<gui_static_text>(box.left+30, y, species->gender_pronoun() + std::string(" ") + stats->intelligence_str(), rltk::colors::WHITE, rltk::colors::DARKEST_GREEN))); ++y;
        dialog->children.push_back(std::move(std::make_unique<gui_static_text>(box.left+30, y, species->gender_pronoun() + std::string(" ") + stats->wisdom_str(), rltk::colors::WHITE, rltk::colors::DARKEST_GREEN))); ++y;
        dialog->children.push_back(std::move(std::make_unique<gui_static_text>(box.left+30, y, species->gender_pronoun() + std::string(" ") + stats->charisma_str(), rltk::colors::WHITE, rltk::colors::DARKEST_GREEN))); ++y;
        dialog->children.push_back(std::move(std::make_unique<gui_static_text>(box.left+30, y, species->gender_pronoun() + std::string(" ") + stats->comeliness_str(), rltk::colors::WHITE, rltk::colors::DARKEST_GREEN))); ++y;
        dialog->children.push_back(std::move(std::make_unique<gui_static_text>(box.left+30, y, species->gender_pronoun() + std::string(" ") + stats->ethics_str(), rltk::colors::WHITE, rltk::colors::DARKEST_GREEN))); ++y;
    } else if (tab == 1) {
        int y = box.top + 8;

        for (const auto &skill : stats->skills) {
            dialog->children.push_back(std::move(std::make_unique<gui_static_text>(box.left + 2, y, skill.first + std::string(" : ") + std::to_string(skill.second.skill_level), rltk::colors::WHITE, rltk::colors::DARKEST_GREEN)));
            ++y;
        }
    } else if (tab == 2) {
        int y = box.top + 8;
        each<item_t, item_carried_t>([&y, &dialog, &box] (entity_t &e, item_t &item, item_carried_t &carried) {
            if (carried.carried_by == selected_settler) {
                const std::string item_info = item.item_name + std::string(" (") + item_loc_name(carried.location) + std::string(")"); 
                dialog->children.push_back(std::move(std::make_unique<gui_static_text>(box.left + 2, y, item_info, rltk::colors::WHITE, rltk::colors::DARKEST_GREEN)));
                ++y;
            }
	    });
    } else if (tab == 3) {
        int y = box.top + 8;
        for (const life_event_t &le : planet.history.settler_life_events[selected_settler]) {
            auto finder = life_event_defs.find(le.type);
            if (finder != life_event_defs.end()) {
                const std::string line = std::to_string(le.year) + std::string(" : ") + finder->second.description;
                dialog->children.push_back(std::move(std::make_unique<gui_static_text>(box.left + 2, y, line, rltk::colors::WHITE, rltk::colors::DARKEST_GREEN)));
                ++y;
            } else {
                std::cout << "Warning: " << le.type << " life event not found\n";
            }
        }
    }

    add_gui_element(std::move(dialog));
}
