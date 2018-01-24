#include "stdafx.h"
#include "sentient_builder.hpp"
#include "../../raws/raws.hpp"
#include "../../raws/materials.hpp"
#include "../../raws/health_factory.hpp"
#include "../../raws/items.hpp"
#include "../../raws/defs/item_def_t.hpp"
#include "../../components/logger.hpp"
#include "../../bengine/string_utils.hpp"
#include "../../components/sentient_ai.hpp"
#include "../../components/game_stats.hpp"
#include "../../components/items/item.hpp"
#include "../../components/renderable.hpp"
#include "../../components/viewshed.hpp"
#include "../../components/initiative.hpp"
#include "../../components/ai_tags/ai_mode_idle.hpp"
#include "../../components/natural_attacks_t.hpp"
#include "../../components/renderable_composite.hpp"
#include "../region/region.hpp"
#include "../../raws/defs/civilization_t.hpp"
#include "../../raws/defs/raw_creature_t.hpp"
#include "../../raws/creatures.hpp"
#include "../../components/riding_t.hpp"
#include "../../components/items/item_carried.hpp"
#include "../../components/claimed_t.hpp"
#include "../../bengine/telemetry.hpp"
#include "../../components/items/item_quality.hpp"
#include "../../components/items/item_wear.hpp"

int sentient_get_stat_mod(const std::string stat, const raw_species_t * species) {
    if (!species) return 0;
    int mod = 0;
    auto finder = species->stat_mods.find(stat);
    if (finder != species->stat_mods.end()) mod = finder->second;
    return mod;
}

void create_sentient(planet_t &planet, bengine::random_number_generator &rng, std::tuple<int,int,int> &start,
                     const civ_unit_sentient_t &unit, const std::string species_tag, const std::size_t civ_id,
                     bool announce)
{
	call_home("Spawn", "Sentient", species_tag);

    // Species definition
    species_t species;
    species.tag = species_tag;
    if (unit.gender == "random") {
        const int gr = rng.roll_dice(1,6);
        if (gr < 4) {
            species.gender = FEMALE;
        } else {
            species.gender = MALE;
        }
    } else if (unit.gender == "female") {
        species.gender = FEMALE;
    } else {
        species.gender = MALE;
    }
    const auto species_finder = get_species_def(species_tag);
    if (species_finder == nullptr) std::cout << "WARNING: Unable to find info for " << species_tag << "\n";
    if (species_finder->render_composite) {
        // We need to define skin color, hair
        species.base_male_glyph = species_finder->base_male_glyph;
        species.base_female_glyph = species_finder->base_female_glyph;
        species.skin_color = planet.civs.civs[civ_id].skin_color;
        species.hair_color = planet.civs.civs[civ_id].hair_color;
        species.hair_style = planet.civs.civs[civ_id].hair_style;
    }

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

    // Natural Attacks
    natural_attacks_t attacks;
    for (const auto &na : unit.natural_attacks) {
        attacks.attacks.push_back( natural_attack_t{ na } );
    }

    // AI
    sentient_ai ai{civ_id, 10};
    if (species_finder->alignment == align_devour) {
        ai.aggression = 100;
        ai.hostile = true;
    }

	auto &[x, y, z] = start;
    auto sentient = bengine::create_entity()
            ->assign(position_t{x, y, z, 0})
            ->assign(name_t{unit.name,
                            ""})
            ->assign(viewshed_t{6, false, false})
            ->assign(std::move(stats))
            ->assign(std::move(health))
            ->assign(std::move(ai))
            ->assign(std::move(species))
            ->assign(initiative_t{})
            ->assign(ai_mode_idle_t{})
            ->assign(std::move(attacks));
    if (species_finder->render_composite) {
        sentient->assign(renderable_composite_t{RENDER_SENTIENT, species_finder->glyph});
    } else {
        sentient->assign(renderable_t{species_finder->glyph, species_finder->glyph_ascii, bengine::color_t{1.0f, 1.0f, 1.0f}, bengine::color_t{0.0f, 0.0f, 0.0f}});
    }
    //std::cout << "Sentient #" << sentient->id << "\n";

    // Starting Equipment
    for (auto item : unit.equipment.starting_clothes) {
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
			spawn_item_carried(sentient->id, cs[0], get_material_by_tag(cs[1]), position, item_quality::AVERAGE, 100);
        }
    }

    if (unit.equipment.melee != "") {
        auto cs = split(unit.equipment.melee, '/');
        const std::string item_name = cs[0];
        auto finder = get_item_def(item_name);
        std::cout << "Created " << item_name << "\n";
        item_t w{item_name};
        w.material = get_material_by_tag(cs[1]);
        w.item_name = material_name(w.material) + std::string(" ") + w.item_name;
        auto weapon = bengine::create_entity()->assign(std::move(w))->assign(item_carried_t{EQUIP_MELEE, sentient->id});
        weapon->component<item_t>()->stack_size = finder->stack_size;
		decorate_item_categories(*weapon, finder->categories);
		weapon->assign(claimed_t{});
    }
    if (unit.equipment.ranged != "") {
        auto cs = split(unit.equipment.ranged, '/');
        const std::string item_name = cs[0];
        auto finder = get_item_def(item_name);
        std::cout << "Created " << item_name << "\n";
        item_t w{item_name};
        w.material = get_material_by_tag(cs[1]);
        w.item_name = material_name(w.material) + std::string(" ") + w.item_name;
        auto weapon = bengine::create_entity()->assign(std::move(w))->assign(item_carried_t{EQUIP_RANGED, sentient->id});
        weapon->component<item_t>()->stack_size = finder->stack_size;
		decorate_item_categories(*weapon, finder->categories);
		weapon->assign(claimed_t{});
	}
    if (unit.equipment.ammo != "") {
        auto cs = split(unit.equipment.ammo, '/');
        const std::string item_name = cs[0];
        auto finder = get_item_def(item_name);
        std::cout << "Created " << item_name << "\n";
        item_t w{item_name};
        w.material = get_material_by_tag(cs[1]);
        w.item_name = material_name(w.material) + std::string(" ") + w.item_name;
        auto ammo = bengine::create_entity()->assign(std::move(w))->assign(item_carried_t{EQUIP_AMMO, sentient->id});
        ammo->component<item_t>()->stack_size = finder->stack_size;
		decorate_item_categories(*ammo, finder->categories);
		ammo->assign(claimed_t{});
    }
    if (unit.equipment.mount != "") {
        std::cout << "Spawning a mount: " << unit.equipment.mount << "\n";
        // Spawn a mount at the same location, with a riding_t tag on the sentient
        auto critter_def = get_creature_def( unit.equipment.mount );

        bool male = true;
        if (rng.roll_dice(1,4)<=2) male = false;
        name_t name{};
        name.first_name = critter_def->name;
        if (male) {
            name.last_name = critter_def->male_name;
        } else {
            name.last_name = critter_def->female_name;
        }

        game_stats_t stats;
        stats.profession_tag = "Mount";
        stats.age = 1;
        for (auto it=critter_def->stats.begin(); it!=critter_def->stats.end(); ++it) {
            if (it->first == "str") stats.strength = it->second;
            if (it->first == "dex") stats.dexterity = it->second;
            if (it->first == "con") stats.constitution = it->second;
            if (it->first == "int") stats.intelligence = it->second;
            if (it->first == "wis") stats.wisdom = it->second;
            if (it->first == "cha") stats.charisma = it->second;
        }

        auto mount = bengine::create_entity()
                ->assign(position_t{x,y,z})
                ->assign(renderable_t{critter_def->glyph, critter_def->glyph_ascii, critter_def->fg, bengine::color_t{0.0f, 0.0f, 0.0f}})
                ->assign(std::move(name))
                ->assign(std::move(stats))
                ->assign(create_health_component_creature(critter_def->tag))
                ->assign(initiative_t{})
                ->assign(ai_mode_idle_t{});

        // Remove the position_t from the parent and assign a riding marker
        sentient->assign(riding_t{mount->id});
    }

    if (announce) {
        //emit_deferred(log_message{
                //LOG().col(bengine::color_t{1.0f, 0.0f, 0.0f})->text(species_finder->name)->text(" ")->col(bengine::color_t{1.0f, 1.0f, 1.0f})->text(
                //        " has arrived.")->chars});
    }

    planet.civs.civs[civ_id].met_cordex = true;

    // Equipment
}

void create_sentient_unit(planet_t &planet, bengine::random_number_generator &rng, std::size_t civ_id,
                          const std::string &unit_tag,
                          std::vector<std::tuple<int,int,int>> &starting_points, int &spawn_counter,
                          const bool announce)
{
    const std::string species_tag = planet.civs.civs[civ_id].species_tag;
    auto civ_f = get_civ_def(species_tag);
    auto unit_f = civ_f->units.find(unit_tag);
    if (unit_f == civ_f->units.end()) {
        std::cout << "Error loading " << unit_tag << "\n";
        return;
    }

    for (const auto &unit : unit_f->second.sentients) {
        for (int i=0; i<unit.n_present; ++i) {
            std::tuple<int,int,int> sp;
            if (starting_points.size() < spawn_counter) {
                sp = starting_points[spawn_counter];
            } else {
                std::get<0>(sp) = rng.roll_dice(1,REGION_WIDTH-1);
                std::get<1>(sp) = rng.roll_dice(1,REGION_HEIGHT-1);
                std::get<2>(sp) = region::ground_z(std::get<0>(sp), std::get<1>(sp));
            }
            create_sentient(planet, rng, sp, unit, species_tag, civ_id, announce);

            ++spawn_counter;
            if (spawn_counter > starting_points.size()) spawn_counter = 0;
        }
    }
}
