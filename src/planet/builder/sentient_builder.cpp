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

int sentient_get_stat_mod(const std::string stat, const raw_species_t * species) {
    int mod = 0;
    auto finder = species->stat_mods.find(stat);
    if (finder != species->stat_mods.end()) mod = finder->second;
    return mod;
}

void create_sentient(planet_t &planet, region_t &region, rltk::random_number_generator &rng, std::tuple<int,int,int> &start,
                     const civ_unit_sentient_t &unit, const std::string species_tag, const std::size_t person_id,
                     bool announce)
{
    // Species definition
    species_t species;
    species.tag = species_tag;
    if (unit.gender == "female") {
        species.gender = FEMALE;
    } else {
        species.gender = MALE;
    }
    const auto species_finder = get_species_def(species_tag);

    // Game stats
    game_stats_t stats;
    stats.strength = rng.roll_dice(3, 6) + sentient_get_stat_mod("str", species_finder);
    stats.dexterity = rng.roll_dice(3, 6) + sentient_get_stat_mod("dex", species_finder);
    stats.constitution = rng.roll_dice(3, 6) + sentient_get_stat_mod("con", species_finder);
    stats.intelligence = rng.roll_dice(3, 6) + sentient_get_stat_mod("int", species_finder);
    stats.wisdom = rng.roll_dice(3, 6) + sentient_get_stat_mod("wis", species_finder);
    stats.charisma = rng.roll_dice(3, 6) + sentient_get_stat_mod("cha", species_finder);
    stats.comeliness = rng.roll_dice(3, 6) + sentient_get_stat_mod("com", species_finder);
    stats.ethics = rng.roll_dice(3, 6) + sentient_get_stat_mod("eth", species_finder);
    stats.age = species_finder->child_age+1;

    // Hit points
    int base_hp = 10;
    for (int i=0; i<unit.base_level; ++i) {
        base_hp += rng.roll_dice(unit.hp_n, unit.hp_dice) + unit.hp_mod;
    }
    health_t health = create_health_component_sentient(species_finder, base_hp);

    // AI
    sentient_ai ai{person_id, 10};
    if (species_finder->alignment == align_devour) {
        ai.aggression = 100;
        ai.hostile = true;
    }

    int x,y,z;
    std::tie(x,y,z) = start;
    auto sentient = create_entity()
            ->assign(position_t{x, y, z})
            ->assign(name_t{unit.name,
                            ""})
            ->assign(renderable_t{species_finder->glyph, rltk::colors::WHITE, rltk::colors::BLACK})
            ->assign(viewshed_t{6, false, false})
            ->assign(std::move(stats))
            ->assign(std::move(health))
            ->assign(std::move(ai))
            ->assign(std::move(species))
            ->assign(initiative_t{})
            ->assign(ai_mode_idle_t{});
    std::cout << "Sentient #" << sentient->id << "\n";
    if (announce) {
        emit_deferred(log_message{
                LOG().col(rltk::colors::CYAN)->text(species_finder->name)->text(" ")->col(rltk::colors::WHITE)->text(
                        " has arrived.")->chars});
    }

    //planet.civs.civs[civ_id].met_cordex = true;

    // Equipment
}

void create_sentient_unit(planet_t &planet, region_t &region, rltk::random_number_generator &rng, std::size_t civ_id,
                          const std::string &unit_tag,
                          std::vector<std::tuple<int,int,int>> &starting_points, int &spawn_counter,
                          const std::size_t person_id,
                          const bool announce)
{
    const std::string species_tag = planet.civs.civs[civ_id].species_tag;
    auto civ_f = civ_defs.find(species_tag);
    auto species_f = get_species_def(civ_f->second.species_tag);
    auto unit_f = civ_f->second.units.find(unit_tag);


    for (const auto &unit : unit_f->second.sentients) {
        for (int i=0; i<unit.n_present; ++i) {
            create_sentient(planet, region, rng, starting_points[spawn_counter], unit, species_tag, person_id, announce);

            ++spawn_counter;
            if (spawn_counter > starting_points.size()) spawn_counter = 0;
        }
    }
}
