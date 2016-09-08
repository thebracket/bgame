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

    //std::cout << species_finder->second.glyph << "\n";
    auto sentient = create_entity()
        ->assign(position_t{x,y,z})
        ->assign(name_t{ species_finder->second.name, OCCUPATION_NAMES[planet.civs.unimportant_people[person_id].occupation] })
        ->assign(renderable_t{ species_finder->second.glyph ,rltk::colors::WHITE, rltk::colors::BLACK })
        ->assign(viewshed_t{ 8, false })
        ->assign(std::move(stats))
        ->assign(std::move(health))
        ->assign(sentient_ai{stat_modifier(stats.dexterity), person_id});

    /*auto sentient = create_entity()
		->assign(position_t{ x,y,z })
		->assign(renderable_t{ species_finder->second.glyph ,rltk::colors::WHITE, rltk::colors::BLACK })
		->assign(name_t{ first_name, last_name })
		->assign(std::move(species))
		->assign(std::move(health))
		->assign(std::move(stats))
		->assign(std::move(ai))
		->assign(viewshed_t{ 8, false });
    */
}
