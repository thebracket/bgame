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

    auto sentient = create_entity()
        ->assign(position_t{x,y,z})
        ->assign(name_t{ species_finder.name, species_finder.castes[planet.civs.population[person_id].caste].name_override })
        ->assign(renderable_t{ species_finder.glyph ,rltk::colors::WHITE, rltk::colors::BLACK })
        ->assign(viewshed_t{ 6, false, false })
        ->assign(std::move(stats))
        ->assign(std::move(health))
        ->assign(sentient_ai{person_id, 10})
        ->assign(std::move(species))
        ->assign(initiative_t{})
        ->assign(ai_mode_idle_t{});
    std::cout << "Sentient #" << sentient->id << "\n";
    if (announce) {
        emit_deferred(log_message{LOG().col(rltk::colors::CYAN)->text(species_finder.name)->text(" ")->col(rltk::colors::WHITE)->text(" has arrived.")->chars});
    }

    planet.civs.civs[planet.civs.population[person_id].civ_id].met_cordex = true;
}
