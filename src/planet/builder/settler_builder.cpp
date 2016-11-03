#include "settler_builder.hpp"
#include "../../components/settler_ai.hpp"
#include "../../components/species.hpp"
#include "../../components/game_stats.hpp"
#include "../../components/name.hpp"
#include "../../components/viewshed.hpp"
#include "../../components/renderable.hpp"
#include "../../components/item.hpp"
#include "../../utils/string_utils.hpp"
#include "../../raws/health_factory.hpp"
#include "../../raws/raws.hpp"
#include "../planet.hpp"

using namespace rltk;

std::vector<std::string> get_event_candidates(const int &age) {
	std::vector<std::string> result;

	for (auto it=life_event_defs.begin(); it!=life_event_defs.end(); ++it) {
		if (age >= it->second.min_age && age <= it->second.max_age) {
			for (int i=0; i<it->second.weight; ++i) {
				result.push_back(it->first);
			}
		}
	}

	return result;
}

void create_settler(planet_t &planet, const int x, const int y, const int z, random_number_generator &rng, int shift_id) {
	species_t species;
	game_stats_t stats;	

	species.tag = "human";

	// Gender
	int gender_roll = rng.roll_dice(1, 21);
	if (gender_roll < 10) {
		species.gender = MALE;
	} else if (gender_roll < 20) {
		species.gender = FEMALE;
	} else {
		species.gender = HERMAPHRODITE;
	}

	// Sexuality
	int sex_roll = rng.roll_dice(1,11);
	if (sex_roll < 9) {
		species.sexuality = HETEROSEXUAL;
	} else if (sex_roll < 10) {
		species.sexuality = HOMOSEXUAL;
	} else {
		species.sexuality = ASEXUAL;
	}

	// Height/Weight
	if (species.gender == MALE || species.gender == HERMAPHRODITE)
	{
		species.height_cm = 147.0F + (rng.roll_dice(2, 10) * 2.5F);
		species.weight_kg = 54.0F + (rng.roll_dice(2, 8) * 0.45);
	}
	else
	{
		species.height_cm = 134.0F + (rng.roll_dice(2, 10) * 2.5F);
		species.weight_kg = 38.0F + (rng.roll_dice(2, 4) * 0.45);
	}

	// Hair/etc. this should be made more realistic one day!
	const int ethnic_roll = rng.roll_dice(1, 4);
	switch (ethnic_roll)
	{
	case 1:
		species.skin_color = CAUCASIAN;
		break;
	case 2:
		species.skin_color = ASIAN;
		break;
	case 3:
		species.skin_color = INDIAN;
		break;
	case 4:
		species.skin_color = AFRICAN;
		break;
	}

	species.bearded = false;
	if (species.gender == MALE)
	{
		const int beard_roll = rng.roll_dice(1, 20);
		if (beard_roll < 7)
		{
			species.bearded = true;
		}
		else
		{
			species.bearded = false;
		}
	}

	const int hair_color_roll = rng.roll_dice(1, 4);
	switch (hair_color_roll)
	{
	case 1:
		species.hair_color = BLACK_HAIR;
		break;
	case 2:
		species.hair_color = BLONDE_HAIR;
		break;
	case 3:
		species.hair_color = BROWN_HAIR;
		break;
	case 4:
		species.hair_color = WHITE_HAIR;
		break;
	}

	species.hair_style = BALD;
	if (species.gender == MALE)
	{
		const int style_roll = rng.roll_dice(1, 5);
		switch (style_roll)
		{
		case 1:
			species.hair_style = BALD;
			break;
		case 2:
			species.hair_style = BALDING;
			break;
		case 3:
			species.hair_style = MOHAWK;
			break;
		case 4:
			species.hair_style = SHORT;
			break;
		case 5:
			species.hair_style = LONG;
			break;
		}
	}
	else
	{
		const int style_roll = rng.roll_dice(1, 4);
		switch (style_roll)
		{
		case 1:
			species.hair_style = SHORT;
			break;
		case 2:
			species.hair_style = LONG;
			break;
		case 3:
			species.hair_style = PIGTAILS;
			break;
		case 4:
			species.hair_style = TRIANGLE;
			break;
		}
	}

	// Name
	std::string first_name;
	if (species.gender == FEMALE) {
		first_name = to_proper_noun_case(first_names_female.random_entry(rng));
	}
	else 
	{
		first_name = to_proper_noun_case(first_names_male.random_entry(rng));
	}

	const std::string last_name = to_proper_noun_case(last_names.random_entry(rng));

	// Profession
	const int number_of_professions = starting_professions.size();
	const std::size_t selected_profession = rng.roll_dice(1,number_of_professions)-1;
	stats.profession_tag = starting_professions[selected_profession].name;

	// Stats
	stats.strength = rng.roll_dice(3,6) + starting_professions[selected_profession].strength;
	stats.dexterity = rng.roll_dice(3,6) + starting_professions[selected_profession].dexterity;
	stats.constitution = rng.roll_dice(3,6) + starting_professions[selected_profession].constitution;
	stats.intelligence = rng.roll_dice(3,6) + starting_professions[selected_profession].intelligence;
	stats.wisdom = rng.roll_dice(3,6)  + starting_professions[selected_profession].wisdom;
	stats.charisma = rng.roll_dice(3,6)  + starting_professions[selected_profession].charisma;
	stats.comeliness = rng.roll_dice(3,6)  + starting_professions[selected_profession].comeliness;
	stats.ethics = rng.roll_dice(3,6)  + starting_professions[selected_profession].ethics;
	stats.age = 15 + rng.roll_dice(3,6);

	auto settler = create_entity();

	// Life events
	int year = 2525 - stats.age;
	int age = 0;
	while (year < 2525) {
		std::vector<std::string> candidates = get_event_candidates(age);
		if (!candidates.empty()) {
			const std::size_t idx = rng.roll_dice(1, candidates.size())-1;
			const std::string event_name = candidates[idx];
			auto ledef = life_event_defs.find(event_name);

			bool has_effect = false;
			if (ledef->second.strength != 0) has_effect = true;
			if (ledef->second.dexterity != 0) has_effect = true;
			if (ledef->second.constitution != 0) has_effect = true;
			if (ledef->second.intelligence != 0) has_effect = true;
			if (ledef->second.wisdom != 0) has_effect = true;
			if (ledef->second.charisma != 0) has_effect = true;
			if (ledef->second.comeliness != 0) has_effect = true;
			if (ledef->second.ethics != 0) has_effect = true;

			if (has_effect) {
				auto finder = planet.history.settler_life_events.find(settler->id);
				const life_event_t event{ year, event_name };
				if (finder == planet.history.settler_life_events.end()) {
					planet.history.settler_life_events[settler->id] = std::vector<life_event_t>{ event };
				} else {
					planet.history.settler_life_events[settler->id].push_back(event);
				}
				stats.strength += ledef->second.strength;
				stats.dexterity += ledef->second.dexterity;
				stats.constitution += ledef->second.constitution;
				stats.intelligence += ledef->second.intelligence;
				stats.wisdom += ledef->second.wisdom;
				stats.charisma += ledef->second.charisma;
				stats.comeliness += ledef->second.comeliness;
				stats.ethics += ledef->second.ethics;
			}
		}

		++year;
		++age;
	}

	int base_hp = rng.roll_dice(1,10) + stat_modifier(stats.constitution);
	if (base_hp < 1) base_hp = 1;
	health_t health = create_health_component_sentient("human", base_hp);

	settler_ai_t ai;
	ai.shift_id = shift_id;

	settler->assign(position_t{ x,y,z })
		->assign(renderable_t{ 1,rltk::colors::YELLOW, rltk::colors::BLACK })
		->assign(name_t{ first_name, last_name })
		->assign(std::move(species))
		->assign(std::move(health))
		->assign(std::move(stats))
		->assign(std::move(ai))
		->assign(viewshed_t{ 8, false });

	// TODO: Create clothing items
	//std::cout << settler->id << "\n";
	for (auto item : starting_professions[selected_profession].starting_clothes) {
		if (std::get<0>(item) == 0 || (std::get<0>(item)==1 && species.gender == MALE) || (std::get<0>(item)==2 && species.gender == FEMALE) ) {
			std::string item_name = std::get<2>(item);
			std::string slot_name = std::get<1>(item);
			item_location_t position = INVENTORY;
			if (slot_name == "head") position = HEAD;
			if (slot_name == "torso") position = TORSO;
			if (slot_name == "legs") position = LEGS;
			if (slot_name == "shoes") position = FEET;
			item_t item{item_name};
			item.material = get_material_by_tag("cloth");
			create_entity()->assign(std::move(item))->assign(item_carried_t{position, settler->id});
		}
	}

	// Add a raygun and energey cells
	auto plasteel = get_material_by_tag("plasteel");
	spawn_item_carried(settler->id, "ray_pistol", plasteel, EQUIP_RANGED);
	spawn_item_carried(settler->id, "small_energy_cell", plasteel, EQUIP_AMMO);
}
