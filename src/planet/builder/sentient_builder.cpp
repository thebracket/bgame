#include "sentient_builder.hpp"
#include "../../raws/raws.hpp"
#include "../../raws/materials.hpp"
#include "../../raws/native_population.hpp"
#include "../../raws/health_factory.hpp"
#include "../../raws/species.hpp"
#include "../../messages/log_message.hpp"
#include "../../components/logger.hpp"
#include "../../utils/string_utils.hpp"
#include "../../components/sentient_ai.hpp"
#include "../../components/species.hpp"
#include "../../components/name.hpp"
#include "../../components/game_stats.hpp"
#include "../../components/item.hpp"
#include "../../components/renderable.hpp"
#include "../../components/viewshed.hpp"
#include "../../components/position.hpp"
#include "../../components/initiative.hpp"
#include "../../components/ai_mode_idle.hpp"

void create_sentient(const int x, const int y, const int z, rltk::random_number_generator &rng, planet_t &planet, 
        region_t &region, const std::size_t person_id, const bool announce) 
{
    species_t species;
    game_stats_t stats;

    species.tag = planet.civs.population[person_id].species_tag;
    std::cout << species.tag << "\n";
    auto species_finder = civ_defs.find(species.tag)->second;
    if (planet.civs.population[person_id].male) {
        species.gender = MALE;
    } else {
        species.gender = FEMALE;
    }

    stats.strength = planet.civs.population[person_id].str;
    stats.dexterity = planet.civs.population[person_id].dex;
    stats.constitution = planet.civs.population[person_id].con;
    stats.intelligence = planet.civs.population[person_id].intelligence;
    stats.wisdom = planet.civs.population[person_id].wis;
    stats.charisma = planet.civs.population[person_id].cha;
    stats.comeliness = planet.civs.population[person_id].com;
    stats.ethics = planet.civs.population[person_id].ethics;
    stats.age = planet.civs.population[person_id].age;

    int base_hp = planet.civs.population[person_id].hit_points;
	if (base_hp < 1) base_hp = 1;
	health_t health = create_health_component_sentient(species.tag, base_hp);


    int techlevel = planet.civs.civs[planet.civs.population[person_id].civ_id].tech_level;
    if (techlevel > 2) techlevel = 2;
    const std::string profession_tag = "heavyinfantry_2";
    std::cout << profession_tag << "\n";
    auto profession = *get_native_professions(profession_tag);
    const int profidx = rng.roll_dice(1,profession.size())-1;    

    auto sentient = create_entity()
        ->assign(position_t{x,y,z})
        ->assign(name_t{ species_finder.name, profession[profidx].name })
        ->assign(renderable_t{ species_finder.glyph ,rltk::colors::WHITE, rltk::colors::BLACK })
        ->assign(viewshed_t{ 6, false, false })
        ->assign(std::move(stats))
        ->assign(std::move(health))
        ->assign(sentient_ai{person_id, profession[profidx].aggression+5})
        ->assign(std::move(species))
        ->assign(initiative_t{})
        ->assign(ai_mode_idle_t{});
    std::cout << "Sentient #" << sentient->id << "\n";
    if (announce) {
        emit_deferred(log_message{LOG().col(rltk::colors::CYAN)->text(species_finder.name)->text(" ")->text(profession[profidx].name)->col(rltk::colors::WHITE)->text(" has arrived.")->chars});
    }

    for (auto item : profession[profidx].starting_clothes) {
		if (std::get<0>(item) == 0 || (std::get<0>(item)==1 && species.gender == MALE) || (std::get<0>(item)==2 && species.gender == FEMALE) ) {            
			std::string item_name = std::get<2>(item);
			std::string slot_name = std::get<1>(item);
            auto cs = split(item_name, '/');
			item_location_t position = INVENTORY;
			if (slot_name == "head") position = HEAD;
			if (slot_name == "torso") position = TORSO;
			if (slot_name == "legs") position = LEGS;
			if (slot_name == "shoes") position = FEET;
            std::cout << "Created " << item_name << "\n";
            item_t clothing{cs[0]};
            clothing.material = get_material_by_tag(cs[1]);
            clothing.item_name = material_name(clothing.material) + std::string(" ") + clothing.item_name;
			create_entity()->assign(std::move(clothing))->assign(item_carried_t{position, sentient->id});
		}
	}

    if (profession[profidx].melee != "") {
        auto cs = split(profession[profidx].melee, '/');
        const std::string item_name = cs[0];
        auto finder = item_defs.find(item_name);
        std::cout << "Created " << item_name << "\n";
        item_t w{item_name};
        w.material = get_material_by_tag(cs[1]);
        w.item_name = material_name(w.material) + std::string(" ") + w.item_name;
        auto weapon = create_entity()->assign(std::move(w))->assign(item_carried_t{EQUIP_MELEE, sentient->id});
        weapon->component<item_t>()->stack_size = finder->second.stack_size;
        weapon->component<item_t>()->category = finder->second.categories;
        weapon->component<item_t>()->claimed = true;
    }
    if (profession[profidx].ranged != "") {
        auto cs = split(profession[profidx].ranged, '/');
        const std::string item_name = cs[0];
        auto finder = item_defs.find(item_name);
        std::cout << "Created " << item_name << "\n";
        item_t w{item_name};
        w.material = get_material_by_tag(cs[1]);
        w.item_name = material_name(w.material) + std::string(" ") + w.item_name;
        auto weapon = create_entity()->assign(std::move(w))->assign(item_carried_t{EQUIP_RANGED, sentient->id});
        weapon->component<item_t>()->stack_size = finder->second.stack_size;
        weapon->component<item_t>()->category = finder->second.categories;
        weapon->component<item_t>()->claimed = true;
    }
    if (profession[profidx].ammo != "") {
        auto cs = split(profession[profidx].ammo, '/');
        const std::string item_name = cs[0];
        auto finder = item_defs.find(item_name);
        std::cout << "Created " << item_name << "\n";
        item_t w{item_name};
        w.material = get_material_by_tag(cs[1]);
        w.item_name = material_name(w.material) + std::string(" ") + w.item_name;
        auto ammo = create_entity()->assign(std::move(w))->assign(item_carried_t{EQUIP_AMMO, sentient->id});
        ammo->component<item_t>()->stack_size = finder->second.stack_size;
        ammo->component<item_t>()->category = finder->second.categories;
        ammo->component<item_t>()->claimed = true;
    }

    planet.civs.civs[planet.civs.population[person_id].civ_id].met_cordex = true;
}
