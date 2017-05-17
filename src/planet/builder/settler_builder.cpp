#include "settler_builder.hpp"
#include "../../components/settler_ai.hpp"
#include "../../components/game_stats.hpp"
#include "../../components/name.hpp"
#include "../../components/viewshed.hpp"
#include "../../components/renderable_composite.hpp"
#include "../../components/item.hpp"
#include "../../utils/string_utils.hpp"
#include "../../raws/raws.hpp"
#include "../../raws/health_factory.hpp"
#include "../../raws/string_table.hpp"
#include "../../raws/life_events.hpp"
#include "../../raws/defs/life_event_template.hpp"
#include "../../raws/profession.hpp"
#include "../../raws/materials.hpp"
#include "../../components/initiative.hpp"
#include "../../components/ai_tags/ai_settler_new_arrival.hpp"
#include "../../components/sleep_clock_t.hpp"
#include "../../components/item_carried.hpp"

using namespace rltk;

std::vector<std::string> get_event_candidates(const int &age, const std::vector<std::string> &past) {
	std::vector<std::string> result;

	std::set<std::string> unavailable;
	for (const std::string &le : past) {
		auto lefinder = get_life_event(le);
		for (const std::string &no : lefinder->precludes_event) {
			unavailable.insert(no);
		}
	}

    each_life_event([&unavailable, &result, &age, &past] (std::string tag, life_event_template * it) {
        if (age >= it->min_age && age <= it->max_age) {
            bool available = true;

            auto nope_check = unavailable.find(tag);
            if (nope_check != unavailable.end()) {
                available = false;
            }

            if (available && !it->requires_event.empty()) {
                available = false;
                for (const auto &req : it->requires_event) {
                    for (const auto &p : past) {
                        if (p == req) available = true;
                    }
                }
            }

            if (available) {
                for (int i=0; i<it->weight; ++i) {
                    result.push_back(tag);
                }
            }

        }
    });

	return result;
}

void create_settler(planet_t &planet, const int x, const int y, const int z, random_number_generator &rng, int shift_id) {
	species_t species;
	game_stats_t stats;
    auto species_def = get_species_def("human");

	species.tag = "human";

	// Gender
	int gender_roll = rng.roll_dice(1, 20);
	if (gender_roll < 11) {
		species.gender = MALE;
	} else {
		species.gender = FEMALE;
	}
	// Sexuality
	int sex_roll = rng.roll_dice(1,10);
	if (sex_roll < 9) {
		species.sexuality = HETEROSEXUAL;
	} else  {
		species.sexuality = HOMOSEXUAL;
	}

	// Height/Weight
	if (species.gender == MALE)
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
    species.base_male_glyph = species_def->base_male_glyph;
    species.base_female_glyph = species_def->base_female_glyph;
    species.skin_color = species_def->skin_colors[rng.roll_dice(1, species_def->skin_colors.size())-1];

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

    species.hair_color = species_def->hair_colors[rng.roll_dice(1, species_def->hair_colors.size())-1];

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
			species.hair_style = SHORT_HAIR;
			break;
		case 5:
			species.hair_style = LONG_HAIR;
			break;
		}
	}
	else
	{
		const int style_roll = rng.roll_dice(1, 4);
		switch (style_roll)
		{
		case 1:
			species.hair_style = SHORT_HAIR;
			break;
		case 2:
			species.hair_style = LONG_HAIR;
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
	using namespace string_tables;
	std::string first_name;
	if (species.gender == FEMALE) {
		first_name = to_proper_noun_case(string_table(FIRST_NAMES_FEMALE)->random_entry(rng));
	}
	else 
	{
		first_name = to_proper_noun_case(string_table(FIRST_NAMES_MALE)->random_entry(rng));
	}

	const std::string last_name = to_proper_noun_case(string_table(LAST_NAMES)->random_entry(rng));

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
	std::vector<std::string> event_buffer;

	while (year < 2522) {
		std::vector<std::string> candidates = get_event_candidates(age, event_buffer);
		if (!candidates.empty()) {
			const std::size_t idx = rng.roll_dice(1, candidates.size())-1;
			const std::string event_name = candidates[idx];
			event_buffer.push_back(event_name);
			auto ledef = get_life_event(event_name);

			bool has_effect = false;
			if (ledef->strength != 0) has_effect = true;
			if (ledef->dexterity != 0) has_effect = true;
			if (ledef->constitution != 0) has_effect = true;
			if (ledef->intelligence != 0) has_effect = true;
			if (ledef->wisdom != 0) has_effect = true;
			if (ledef->charisma != 0) has_effect = true;
			if (ledef->comeliness != 0) has_effect = true;
			if (ledef->ethics != 0) has_effect = true;
			if (!ledef->skills.empty()) has_effect = true;

			if (age==0 || has_effect) {
				auto finder = planet.history.settler_life_events.find(settler->id);
				const life_event_t event{ year, event_name };
				if (finder == planet.history.settler_life_events.end()) {
					planet.history.settler_life_events[settler->id] = std::vector<life_event_t>{ event };
				} else {
					planet.history.settler_life_events[settler->id].push_back(event);
				}
				if (rng.roll_dice(1,10)>7) {
					stats.strength += ledef->strength;
					stats.dexterity += ledef->dexterity;
					stats.constitution += ledef->constitution;
					stats.intelligence += ledef->intelligence;
					stats.wisdom += ledef->wisdom;
					stats.charisma += ledef->charisma;
					stats.comeliness += ledef->comeliness;
					stats.ethics += ledef->ethics;
				}
				for (const std::string &skill : ledef->skills) {
                    if (rng.roll_dice(1,10)>7) {
                        auto skillfinder = stats.skills.find(skill);
                        if (skillfinder == stats.skills.end()) {
                            stats.skills[skill] = skill_t{1, 0};
                        } else {
                            if (stats.skills[skill].skill_level < 3) {
                                ++stats.skills[skill].skill_level;
                                std::cout << "Raised skill in " << skill << " to " << +stats.skills[skill].skill_level << "\n";
                            }
                        }
                    }
				}
			}
		}

		++year;
		++age;
	}
	planet.history.settler_life_events[settler->id].push_back(life_event_t{2524, "ark_b"});
	planet.history.settler_life_events[settler->id].push_back(life_event_t{2525, "planetfall"});

	int base_hp = rng.roll_dice(1,10) + stat_modifier(stats.constitution);
	if (base_hp < 1) base_hp = 1;
	health_t health = create_health_component_settler("human", base_hp);

	settler_ai_t ai;
	ai.shift_id = shift_id;

	settler->assign(position_t{ x,y,z })
		->assign(renderable_composite_t{ RENDER_SETTLER, 2 })
		->assign(name_t{ first_name, last_name })
		->assign(std::move(species))
		->assign(std::move(health))
		->assign(std::move(stats))
		->assign(std::move(ai))
		->assign(viewshed_t{ 8, false })
		->assign(initiative_t{})
		->assign(ai_settler_new_arrival_t{})
		->assign(sleep_clock_t{});

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
