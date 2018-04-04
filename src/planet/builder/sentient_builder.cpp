#include "sentient_builder.hpp"
#include "../../raws/raws.hpp"
#include "../../raws/materials.hpp"
#include "../../raws/health_factory.hpp"
#include "../../raws/items.hpp"
#include "../../raws/defs/item_def_t.hpp"
#include "../../bengine/string_utils.hpp"
#include "../region/region.hpp"
#include "../../raws/defs/civilization_t.hpp"
#include "../../raws/defs/raw_creature_t.hpp"
#include "../../raws/creatures.hpp"
#include "../../bengine/telemetry.hpp"
#include "../../bengine/geometry.hpp"

static int sentient_get_stat_mod(const std::string &stat, const raw_species_t * species) noexcept {
    if (!species) return 0;
    auto mod = 0;
    const auto finder = species->stat_mods.find(stat);
    if (finder != species->stat_mods.end()) mod = finder->second;
    return mod;
}

static void create_sentient(planet_t &planet, bengine::random_number_generator &rng, std::tuple<int,int,int> &start,
                     const civ_unit_sentient_t &unit, const std::string &species_tag, const std::size_t civ_id,
                     bool announce) noexcept
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
    if (species_finder == nullptr) glog(log_target::GAME, log_severity::warning, "Unable to find info for {0}", species_tag);
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
    auto base_hp = 10;
    for (auto i=0; i<unit.base_level; ++i) {
        base_hp += rng.roll_dice(unit.hp_n, unit.hp_dice) + unit.hp_mod;
    }
	auto health = create_health_component_sentient(species_finder, base_hp);

    // Natural Attacks
    natural_attacks_t attacks;
    for (const auto &na : unit.natural_attacks) {
        attacks.attacks.emplace_back( natural_attack_t{ na } );
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
			const auto item_name = std::get<2>(item);
			const auto slot_name = std::get<1>(item);
            auto cs = split(item_name, '/');
			auto position = INVENTORY;
            if (slot_name == "head") position = HEAD;
            if (slot_name == "torso") position = TORSO;
            if (slot_name == "legs") position = LEGS;
            if (slot_name == "shoes") position = FEET;
            //std::cout << "Created " << item_name << "\n";
			spawn_item_carried(sentient->id, cs[0], get_material_by_tag(cs[1]), position, item_quality::AVERAGE, 100, 0, planet.civs.civs[civ_id].name);
        }
    }

    if (!unit.equipment.melee.empty()) {
        auto cs = split(unit.equipment.melee, '/');
        const auto item_name = cs[0];
		spawn_item_carried(sentient->id, cs[0], get_material_by_tag(cs[1]), EQUIP_MELEE, 3, 100, 0, planet.civs.civs[civ_id].name);
    }
    if (!unit.equipment.ranged.empty()) {
        auto cs = split(unit.equipment.ranged, '/');
        const auto item_name = cs[0];
		spawn_item_carried(sentient->id, cs[0], get_material_by_tag(cs[1]), EQUIP_RANGED, 3, 100, 0, planet.civs.civs[civ_id].name);
	}
    if (!unit.equipment.ammo.empty()) {
        auto cs = split(unit.equipment.ammo, '/');
        const auto item_name = cs[0];
		spawn_item_carried(sentient->id, cs[0], get_material_by_tag(cs[1]), EQUIP_AMMO, 3, 100, 0, planet.civs.civs[civ_id].name);
    }
    if (!unit.equipment.mount.empty()) {
        //std::cout << "Spawning a mount: " << unit.equipment.mount << "\n";
        // Spawn a mount at the same location, with a riding_t tag on the sentient
        const auto critter_def = get_creature_def( unit.equipment.mount );

		auto male = true;
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
		for (auto &it : critter_def->stats) {
            if (it.first == "str") stats.strength = it.second;
            if (it.first == "dex") stats.dexterity = it.second;
            if (it.first == "con") stats.constitution = it.second;
            if (it.first == "int") stats.intelligence = it.second;
            if (it.first == "wis") stats.wisdom = it.second;
            if (it.first == "cha") stats.charisma = it.second;
        }

        const auto mount = bengine::create_entity()
                ->assign(position_t{x,y,z})
                ->assign(renderable_t{critter_def->glyph, critter_def->glyph_ascii, critter_def->fg, bengine::color_t{0.0f, 0.0f, 0.0f}, critter_def->vox})
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
                          const bool announce, int crash_x, int crash_y, int crash_z) noexcept
{
    const std::string species_tag = planet.civs.civs[civ_id].species_tag;
    auto civ_f = get_civ_def(species_tag);
    auto unit_f = civ_f->units.find(unit_tag);
    if (unit_f == civ_f->units.end()) {
		glog(log_target::GAME, log_severity::error, "Error loading unit: {0}", unit_tag);
        return;
    }

    for (const auto &unit : unit_f->second.sentients) {
        for (int i=0; i<unit.n_present; ++i) {
            std::tuple<int,int,int> sp;
            if (starting_points.size() < spawn_counter) {
                sp = starting_points[spawn_counter];
            } else {
				bool ok = false;
				while (!ok) {
					std::get<0>(sp) = rng.roll_dice(1, REGION_WIDTH - 1);
					std::get<1>(sp) = rng.roll_dice(1, REGION_HEIGHT - 1);
					std::get<2>(sp) = region::ground_z(std::get<0>(sp), std::get<1>(sp));

					const int sidx = mapidx(std::get<0>(sp), std::get<1>(sp), std::get<2>(sp));

					const auto distance = bengine::distance3d(std::get<0>(sp), std::get<1>(sp), std::get<2>(sp), crash_x, crash_y, crash_z);
					if (region::water_level(sidx) < 1 && region::tree_id(sidx) == 0 && distance > 8.0f) ok = true;
				}
            }
            create_sentient(planet, rng, sp, unit, species_tag, civ_id, announce);

            ++spawn_counter;
            if (spawn_counter > starting_points.size()) spawn_counter = 0;
        }
    }
}
