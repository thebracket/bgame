#include "sentient_builder.hpp"
#include "../../raws/raws.hpp"
#include "../../components/components.hpp"
#include "../../raws/health_factory.hpp"

int sentient_stat_mod(boost::container::flat_map<std::string, raw_species_t>::iterator &species, const std::string &stat) {
    auto finder = species->second.stat_mods.find(stat);
    if (finder == species->second.stat_mods.end()) return 0;
    return finder->second;
}

void create_sentient(const int x, const int y, const int z, rltk::random_number_generator &rng, planet_t &planet, region_t &region, const std::size_t person_id) 
{
    species_t species;
    game_stats_t stats;

    species.tag = planet.civs.unimportant_people[person_id].species_tag;
    auto species_finder = species_defs.find(species.tag);
    if (species_finder == species_defs.end()) {
        std::cout << "WARNING: Could not find raws for [" << species.tag << "]\n";
        return;
    }
    if (planet.civs.unimportant_people[person_id].male) {
        species.gender = MALE;
    } else {
        species.gender = FEMALE;
    }

    stats.strength = rng.roll_dice(3,6) + sentient_stat_mod(species_finder, "str");
    stats.dexterity = rng.roll_dice(3,6) + sentient_stat_mod(species_finder, "dex");
    stats.constitution = rng.roll_dice(3,6) + sentient_stat_mod(species_finder, "con");
    stats.intelligence = rng.roll_dice(3,6) + sentient_stat_mod(species_finder, "int");
    stats.wisdom = rng.roll_dice(3,6) + sentient_stat_mod(species_finder, "wis");
    stats.charisma = rng.roll_dice(3,6) + sentient_stat_mod(species_finder, "cha");
    stats.comeliness = rng.roll_dice(3,6) + sentient_stat_mod(species_finder, "com");
    stats.ethics = rng.roll_dice(3,6) + sentient_stat_mod(species_finder, "eth");
    stats.age = planet.civs.unimportant_people[person_id].age;

    int base_hp = rng.roll_dice(1,10) + stat_modifier(stats.constitution);
	if (base_hp < 1) base_hp = 1;
	health_t health = create_health_component_sentient(species.tag, base_hp);


    int techlevel = planet.civs.civs[planet.civs.unimportant_people[person_id].civ_id].tech_level;
    if (techlevel > 2) techlevel = 2;
    const std::string profession_tag = OCCUPATION_TAGS[planet.civs.unimportant_people[person_id].occupation] + std::string("_") + 
        std::to_string(techlevel);
    std::cout << profession_tag << "\n";
    auto profession = native_pop_defs.find(profession_tag);
    if (profession == native_pop_defs.end()) {
        throw std::runtime_error(std::string("Cannot find ") + profession_tag);
    }
    const int profidx = rng.roll_dice(1,profession->second.size())-1;    

    //std::cout << species_finder->second.glyph << "\n";
    auto sentient = create_entity()
        ->assign(position_t{x,y,z})
        ->assign(name_t{ species_finder->second.name, profession->second[profidx].name })
        ->assign(renderable_t{ species_finder->second.glyph ,rltk::colors::WHITE, rltk::colors::BLACK })
        ->assign(viewshed_t{ 6, false, false })
        ->assign(std::move(stats))
        ->assign(std::move(health))
        ->assign(sentient_ai{stat_modifier(stats.dexterity), person_id, profession->second[profidx].aggression+5});
    std::cout << "Sentient #" << sentient->id << "\n";

    for (auto item : profession->second[profidx].starting_clothes) {
		if (std::get<0>(item) == 0 || (std::get<0>(item)==1 && species.gender == MALE) || (std::get<0>(item)==2 && species.gender == FEMALE) ) {
			std::string item_name = std::get<2>(item);
			std::string slot_name = std::get<1>(item);
			item_location_t position = INVENTORY;
			if (slot_name == "head") position = HEAD;
			if (slot_name == "torso") position = TORSO;
			if (slot_name == "legs") position = LEGS;
			if (slot_name == "shoes") position = FEET;
            std::cout << "Created " << item_name << "\n";
			create_entity()->assign(item_t{item_name})->assign(item_carried_t{position, sentient->id});
		}
	}

    if (profession->second[profidx].melee != "") {
        const std::string item_name = profession->second[profidx].melee;
        std::cout << "Created " << item_name << "\n";
        create_entity()->assign(item_t{item_name})->assign(item_carried_t{EQUIP_MELEE, sentient->id});
    }
    if (profession->second[profidx].ranged != "") {
        const std::string item_name = profession->second[profidx].ranged;
        std::cout << "Created " << item_name << "\n";
        create_entity()->assign(item_t{item_name})->assign(item_carried_t{EQUIP_RANGED, sentient->id});
    }
    if (profession->second[profidx].ammo != "") {
        const std::string item_name = profession->second[profidx].ammo;
        std::cout << "Created " << item_name << "\n";
        create_entity()->assign(item_t{item_name})->assign(item_carried_t{EQUIP_AMMO, sentient->id});
    }

    planet.civs.civs[planet.civs.unimportant_people[person_id].civ_id].met_cordex = true;
}
