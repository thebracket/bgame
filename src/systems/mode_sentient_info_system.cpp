#include "mode_sentient_info_system.hpp"
#include "../main/game_globals.hpp"
#include "../raws/raws.hpp"
#include "gui_system.hpp"
#include "../messages/map_dirty_message.hpp"
#include "../messages/recalculate_mining_message.hpp"

void mode_sentient_info_system::configure() { 
    system_name = "Creature Info System";   
}

void display_sentient_info() {
    dialog_placement_t box{};

    auto name = entity(selected_settler)->component<name_t>();
	auto species = entity(selected_settler)->component<species_t>();
	auto ai = entity(selected_settler)->component<sentient_ai>();
    auto health = entity(selected_settler)->component<health_t>();
    auto species_finder = species_defs.find(species->tag);
    if (species_finder == species_defs.end()) std::cout << "WARNING: Could not find tag for species: " << species->tag << "\n";

	std::stringstream header;
	header << " " << name->first_name << " " << name->last_name;

    std::stringstream header2;
    header2 << species->gender_str() << " " << species_finder->second.name;

    std::stringstream header3;
    header3 << "Member of: " << planet.civs.civs[planet.civs.unimportant_people[ai->person_id].civ_id].name;
    if (planet.civs.civs[planet.civs.unimportant_people[ai->person_id].civ_id].cordex_feelings < 0) {
        header3 << " (HOSTILE)";        
    } else if (planet.civs.civs[planet.civs.unimportant_people[ai->person_id].civ_id].cordex_feelings == 0) {
        header3 << " (NEUTRAL)";
    } else {
        header3 << " (FRIENDLY)";
    }
    header3 << ".";

    std::unique_ptr<gui_dialog> dialog = std::make_unique<gui_dialog>(header.str(), [] () { 
        // On close
        game_master_mode = PLAY;
        emit_deferred(map_dirty_message{});
        emit_deferred(recalculate_mining_message{});
    });
    dialog->children.push_back(std::move(std::make_unique<gui_static_text>(box.left+2, box.top+2, header2.str(), rltk::colors::MAGENTA, rltk::colors::DARKEST_GREEN)));
    dialog->children.push_back(std::move(std::make_unique<gui_static_text>(box.left+2, box.top+3, species_finder->second.description, rltk::colors::MAGENTA, rltk::colors::DARKEST_GREEN)));
    dialog->children.push_back(std::move(std::make_unique<gui_static_text>(box.left+2, box.top+4, header3.str(), rltk::colors::MAGENTA, rltk::colors::DARKEST_GREEN)));

    // Display health
    int y = box.top+6;
    dialog->children.push_back(std::move(std::make_unique<gui_static_text>(box.left+2, y, "Hit Points: " + std::to_string(health->current_hitpoints) + std::string("/") + std::to_string(health->max_hitpoints), rltk::colors::RED, rltk::colors::DARKEST_GREEN)));
    ++y;
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

    // Display inventory
    each<item_t, item_carried_t>([&y, &dialog, &box] (entity_t &e, item_t &item, item_carried_t &carried) {
        if (carried.carried_by == selected_settler) {
            const std::string item_info = item.item_name + std::string(" (") + item_loc_name(carried.location) + std::string(")"); 
            dialog->children.push_back(std::move(std::make_unique<gui_static_text>(box.left + 2, y, item_info, rltk::colors::WHITE, rltk::colors::DARKEST_GREEN)));
            ++y;
        }
    });

    add_gui_element(std::move(dialog));
}

void display_grazer_info() {
    dialog_placement_t box{};

    auto name = entity(selected_settler)->component<name_t>();
	auto species = entity(selected_settler)->component<species_t>();
	auto ai = entity(selected_settler)->component<sentient_ai>();
    auto health = entity(selected_settler)->component<health_t>();
    auto species_finder = creature_defs.find(species->tag);
    if (species_finder == creature_defs.end()) std::cout << "WARNING: Could not find tag for species: " << species->tag << "\n";

	std::stringstream header;
	header << " " << name->first_name << " " << name->last_name;

    std::stringstream header2;
    header2 << species->gender_str() << " " << species_finder->second.name;

    std::unique_ptr<gui_dialog> dialog = std::make_unique<gui_dialog>(header.str(), [] () { 
        // On close
        game_master_mode = PLAY;
        emit_deferred(map_dirty_message{});
        emit_deferred(recalculate_mining_message{});
    });
    dialog->children.push_back(std::move(std::make_unique<gui_static_text>(box.left+2, box.top+2, header2.str(), rltk::colors::MAGENTA, rltk::colors::DARKEST_GREEN)));
    dialog->children.push_back(std::move(std::make_unique<gui_static_text>(box.left+2, box.top+3, species_finder->second.description, rltk::colors::MAGENTA, rltk::colors::DARKEST_GREEN)));

    // Display health
    int y = box.top+6;
    dialog->children.push_back(std::move(std::make_unique<gui_static_text>(box.left+2, y, "Hit Points: " + std::to_string(health->current_hitpoints) + std::string("/") + std::to_string(health->max_hitpoints), rltk::colors::RED, rltk::colors::DARKEST_GREEN)));
    ++y;
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

    // Display inventory
    each<item_t, item_carried_t>([&y, &dialog, &box] (entity_t &e, item_t &item, item_carried_t &carried) {
        if (carried.carried_by == selected_settler) {
            const std::string item_info = item.item_name + std::string(" (") + item_loc_name(carried.location) + std::string(")"); 
            dialog->children.push_back(std::move(std::make_unique<gui_static_text>(box.left + 2, y, item_info, rltk::colors::WHITE, rltk::colors::DARKEST_GREEN)));
            ++y;
        }
    });

    add_gui_element(std::move(dialog));
}

void mode_sentient_info_system::update(const double ms) {
    if (game_master_mode != SENTIENT_INFO && game_master_mode != GRAZER_INFO) return;

    if (game_master_mode == SENTIENT_INFO) {
        display_sentient_info();
    } else {
        display_grazer_info();
    }
}
