#include "world_gen_settlers.hpp"
#include "../../../engine/colors.hpp"
#include "../tables.h"
#include "../../raws/raw_clothing.h"
#include "../../raws/raws.h"

using engine::entity;
using namespace engine;

entity make_settler(const location_t &loc)
{
	entity test = ECS->add_entity();

	ECS->add_component(test, debug_name_component("Test"));
	ECS->add_component(test, position_component3d(loc, OMNI));
	//engine::globals::ecs->add_component(test, obstruction_component(true,false));
	ECS->add_component(test,
			renderable_component('@', yellow, black, 34, 1, false, true));
	ECS->add_component(test, viewshed_component(visibility, 12));
	settler_ai_component ai;

	std::pair<string, raws::base_raw *> profession =
			raws::get_random_starting_profession();

	// Create some basic attributes
	game_stats_component stats;
	stats.strength = game_engine->rng.roll_dice(3, 6)
			+ profession.second->get_modifier("Strength");
	stats.dexterity = game_engine->rng.roll_dice(3, 6)
			+ profession.second->get_modifier("Dexterity");
	stats.constitution = game_engine->rng.roll_dice(3, 6)
			+ profession.second->get_modifier("Constitution");
	stats.intelligence = game_engine->rng.roll_dice(3, 6)
			+ profession.second->get_modifier("Intelligence");
	stats.wisdom = game_engine->rng.roll_dice(3, 6)
			+ profession.second->get_modifier("Wisdom");
	stats.charisma = game_engine->rng.roll_dice(3, 6)
			+ profession.second->get_modifier("Charisma");
	stats.comeliness = game_engine->rng.roll_dice(3, 6)
			+ profession.second->get_modifier("Comeliness");
	stats.ethics = game_engine->rng.roll_dice(3, 6)
			+ profession.second->get_modifier("Ethics");
	stats.age = 16 + game_engine->rng.roll_dice(3, 6);

	game_species_component species;
	species.species = "Human";

	int gender_roll = game_engine->rng.roll_dice(1, 101);
	if (gender_roll <= 50)
	{
		species.gender = gender_t::MALE;
	}
	else if (gender_roll <= 100)
	{
		species.gender = gender_t::FEMALE;
	}
	else
	{
		species.gender = gender_t::HERMAPHRODITE;
	}

	int preference_roll = game_engine->rng.roll_dice(1, 100);
	if (preference_roll < 92)
	{
		species.sexual_preference = preference_t::HETEROSEXUAL;
	}
	else if (preference_roll < 94)
	{
		species.sexual_preference = preference_t::BISEXUAL;
	}
	else if (preference_roll < 99)
	{
		species.sexual_preference = preference_t::HOMOSEXUAL;
	}
	else
	{
		species.sexual_preference = preference_t::ASEXUAL;
	}

	float height_cm, weight_kg;

	if (species.gender == gender_t::MALE
			or species.gender == gender_t::HERMAPHRODITE)
	{
		height_cm = 147.0F + (game_engine->rng.roll_dice(2, 10) * 2.5F);
		weight_kg = 54.0F + (game_engine->rng.roll_dice(2, 8) * 0.45);
		ai.calories = 4400 + game_engine->rng.roll_dice(1, 200);
		ai.calorie_burn_at_rest = 1 + game_engine->rng.roll_dice(1, 2);
	}
	else
	{
		height_cm = 134.0F + (game_engine->rng.roll_dice(2, 10) * 2.5F);
		weight_kg = 38.0F + (game_engine->rng.roll_dice(2, 4) * 0.45);
		ai.calories = 3600 + game_engine->rng.roll_dice(1, 200);
		ai.calorie_burn_at_rest = game_engine->rng.roll_dice(1, 2);
	}
	species.height_cm = height_cm;
	species.weight_kg = weight_kg;
	ai.wakefulness = 1800 + game_engine->rng.roll_dice(1, 100)
			+ (stat_modifier(stats.constitution) * 3);
	ai.thirst = 960 + game_engine->rng.roll_dice(1, 20);

	ai.first_name = raws::get_random_first_name(species.gender);
	ai.last_name = raws::get_random_last_name();
	ai.profession_tag = profession.first;

	game_health_component health;
	health.max_hit_points = 8 + stat_modifier(stats.constitution); // First level, they get max!
	if (health.max_hit_points < 1)
		health.max_hit_points = 1;
	health.current_hit_points = health.max_hit_points;

	ai.state_major = IDLE;
	ai.state_timer = 0;

	// Hair/etc. this should be made more realistic one day!
	const int ethnic_roll = game_engine->rng.roll_dice(1, 4);
	switch (ethnic_roll)
	{
	case 1:
		species.skin_color = skin_color_t::CAUCASIAN;
		break;
	case 2:
		species.skin_color = skin_color_t::ASIAN;
		break;
	case 3:
		species.skin_color = skin_color_t::INDIAN;
		break;
	case 4:
		species.skin_color = skin_color_t::AFRICAN;
		break;
	}

	species.bearded = false;
	if (species.gender == gender_t::MALE)
	{
		const int beard_roll = game_engine->rng.roll_dice(1, 20);
		if (beard_roll < 7)
		{
			species.bearded = true;
		}
		else
		{
			species.bearded = false;
		}
	}

	const int hair_color_roll = game_engine->rng.roll_dice(1, 4);
	switch (hair_color_roll)
	{
	case 1:
		species.hair_color = hair_color_t::BLACK;
		break;
	case 2:
		species.hair_color = hair_color_t::BLONDE;
		break;
	case 3:
		species.hair_color = hair_color_t::BROWN;
		break;
	case 4:
		species.hair_color = hair_color_t::WHITE;
		break;
	}

	species.hair_style = hair_style_t::BALD;
	if (species.gender == gender_t::MALE)
	{
		const int style_roll = game_engine->rng.roll_dice(1, 5);
		switch (style_roll)
		{
		case 1:
			species.hair_style = hair_style_t::BALD;
			break;
		case 2:
			species.hair_style = hair_style_t::BALDING;
			break;
		case 3:
			species.hair_style = hair_style_t::MOHAWK;
			break;
		case 4:
			species.hair_style = hair_style_t::SHORT;
			break;
		case 5:
			species.hair_style = hair_style_t::LONG;
			break;
		}
	}
	else
	{
		const int style_roll = game_engine->rng.roll_dice(1, 4);
		switch (style_roll)
		{
		case 1:
			species.hair_style = hair_style_t::SHORT;
			break;
		case 2:
			species.hair_style = hair_style_t::LONG;
			break;
		case 3:
			species.hair_style = hair_style_t::PIGTAILS;
			break;
		case 4:
			species.hair_style = hair_style_t::TRIANGLE;
			break;
		}
	}

	ECS->add_component(test, ai);
	ECS->add_component(test, stats);
	ECS->add_component(test, health);
	ECS->add_component(test, species);
	//ECS->add_component(test, point_light_component(6));

	// Make clothing for the settler
	//std::cout << "Generating: " << profession.first << "\n";
	for (const std::unique_ptr<raws::base_raw> &raw : profession.second->children)
	{
		if (raw->type == raws::CLOTHING)
		{
			raws::raw_clothing * rawc =
					static_cast<raws::raw_clothing *>(raw.get());
			//std::cout << "Clothing: (" << rawc->slot << "): " << rawc->item << ", gender " << rawc->gender << "\n";
			if (rawc->gender == 0
					or (species.gender == gender_t::FEMALE and rawc->gender == 2)
					or (species.gender != gender_t::FEMALE and rawc->gender != 2))
			{
				//std::cout << " ----- " << rawc->item << " - " << rawc->slot << " - " << rawc->type << "\n";
				const int item_entity_id = raws::create_item_from_raws(
						rawc->item);
				int position = 0;

				if (rawc->slot == "Head")
					position = 1;
				if (rawc->slot == "Torso")
					position = 2;
				if (rawc->slot == "Legs")
					position = 3;
				if (rawc->slot == "Shoes")
					position = 4;

				item_carried_component carried(test.handle, position);
				ECS->add_component<item_carried_component>(
						*ECS->get_entity_by_handle(item_entity_id), carried);

				item_component * item = ECS->find_entity_component<
						item_component>(item_entity_id);
				item->clothing_slot = position;
			}
		}
	}

	return test;
}
