#include "game_stats.hpp"
#include "../messages/log_message.hpp"
#include "../components/logger.hpp"
#include "../utils/serialization_wrapper.hpp"

void game_stats_t::save(std::ostream &lbfile) {
    Serialize("game_stats_t", lbfile, profession_tag, strength, dexterity, constitution, intelligence, wisdom, charisma, comeliness, ethics, age, skills);
}

game_stats_t game_stats_t::load(std::istream &lbfile) {
    game_stats_t c;
    Deserialize("game_stats_t", lbfile, c.profession_tag, c.strength, c.dexterity, c.constitution, c.intelligence, c.wisdom, c.charisma, c.comeliness, c.ethics, c.age, c.skills);
    return c;
}

std::string game_stats_t::strength_str() {
    if (strength < 5) return "is very weak.";
    if (strength < 8) return "is weak.";
    if (strength < 12) return "is of average strength.";
    if (strength < 15) return "is strong.";
    if (strength < 18) return "is very strong.";
    return "has super-human strength";
}

std::string game_stats_t::dexterity_str() {
    if (dexterity < 5) return "is very clumsy.";
    if (dexterity < 8) return "is clumsy.";
    if (dexterity < 12) return "is of average dexterity.";
    if (dexterity < 15) return "is agile.";
    if (dexterity < 18) return "is very agile.";
    return "has super-human dexterity";
}

std::string game_stats_t::constitution_str() {
    if (constitution < 5) return "is very unhealthy.";
    if (constitution < 8) return "is unhealthy.";
    if (constitution < 12) return "is of average constitution.";
    if (constitution < 15) return "is healthy.";
    if (constitution < 18) return "is very healthy.";
    return "has super-human constitution";
}

std::string game_stats_t::intelligence_str() {
    if (intelligence < 5) return "is very stupid.";
    if (intelligence < 8) return "is stupid.";
    if (intelligence < 12) return "is of average intelligence.";
    if (intelligence < 15) return "is intelligent.";
    if (intelligence < 18) return "is very intelligent.";
    return "has super-human intelligence";
}

std::string game_stats_t::wisdom_str() {
    if (wisdom < 5) return "is very gullible.";
    if (wisdom < 8) return "is gullible.";
    if (wisdom < 12) return "is of average wisdom.";
    if (wisdom < 15) return "is wise.";
    if (wisdom < 18) return "is very wise.";
    return "has super-human wisdom";
}

std::string game_stats_t::charisma_str() {
    if (charisma < 5) return "makes everyone uncomfortable.";
    if (charisma < 8) return "is socially awkward.";
    if (charisma < 12) return "is of average charisma.";
    if (charisma < 15) return "is socially adept.";
    if (charisma < 18) return "is always the center of attention.";
    return "has super-human charisma";
}

std::string game_stats_t::comeliness_str() {
    if (comeliness < 5) return "is incredibly ugly.";
    if (comeliness < 8) return "is ugly.";
    if (comeliness < 12) return "is of average comeliness.";
    if (comeliness < 15) return "is good looking.";
    if (comeliness < 18) return "could be a supermodel.";
    return "has super-human appearance";
}

std::string game_stats_t::ethics_str() {
    if (ethics < 5) return "would sell grandma for a profit.";
    if (ethics < 8) return "has a weak sense of justice.";
    if (ethics < 12) return "is of average ethics.";
    if (ethics < 15) return "has a strong sense of right and wrong.";
    if (ethics < 18) return "is a paragon of virtue.";
    return "is unbending in their beliefs, a modern-day Paladin.";
}

int get_attribute_modifier_for_skill(const game_stats_t &stats, const std::string &skill) {
	auto finder = skill_table.find(skill);
	if (finder != skill_table.end()) {
		switch (finder->second) {
			case strength : return stat_modifier(stats.strength);
			case dexterity : return stat_modifier(stats.dexterity);
			case constitution : return stat_modifier(stats.constitution);
			case intelligence : return stat_modifier(stats.intelligence);
			case wisdom : return stat_modifier(stats.wisdom);
			case charisma : return stat_modifier(stats.charisma);
			case ethics : return stat_modifier(stats.ethics);
			default: return 0;
		}
	} else {
		std::cout << "Warning: unknown skill [" << skill << "]\n";
		return 0;
	}
}

int8_t get_skill_modifier(const game_stats_t &stats, const std::string &skill) {
	auto finder = stats.skills.find(skill);
	if (finder != stats.skills.end()) {
		return finder->second.skill_level;
	} else {
		return 0;
	}
}

void gain_skill_from_success(const std::size_t settler_id, game_stats_t &stats, const std::string &skill, const int &difficulty, rltk::random_number_generator &rng) {
	auto finder = stats.skills.find(skill);
	if (finder != stats.skills.end()) {
		finder->second.experience_gained += difficulty;
		const int gain_bonus_at = (finder->second.skill_level+1) * 100;
		if (finder->second.experience_gained > gain_bonus_at) {
			emit_deferred(log_message{LOG{}.settler_name(settler_id)->text(" has improved their skill in ")->text(skill)->chars});
			finder->second.experience_gained = 0;
			++finder->second.skill_level;

			if (rng.roll_dice(1,6)==1) {
				auto relevant_attribute = skill_table.find(skill);
				if (relevant_attribute != skill_table.end()) {
					const int stat_gain_roll = rng.roll_dice(3,6);
					int attribute_target = 0;
					switch (relevant_attribute->second) {
						case strength : attribute_target = stats.strength; break;
						case dexterity : attribute_target = stats.dexterity; break;
						case constitution : attribute_target = stats.constitution; break;
						case intelligence : attribute_target = stats.intelligence; break;
						case wisdom : attribute_target = stats.wisdom; break;
						case charisma : attribute_target = stats.charisma; break;
						case ethics : attribute_target = stats.ethics; break;
					}
					if (stat_gain_roll < attribute_target) {
						emit_deferred(log_message{LOG{}.settler_name(settler_id)->text(" has gained an attribute point.")->chars});
						switch (relevant_attribute->second) {
							case strength : ++stats.strength; break;
							case dexterity : ++stats.dexterity; break;
							case constitution : ++stats.constitution; break;
							case intelligence : ++stats.intelligence; break;
							case wisdom : ++stats.wisdom; break;
							case charisma : ++stats.charisma; break;
							case ethics : ++stats.ethics; break;
						}
					}
				}
			}
		}
	} else {
		stats.skills[skill] = skill_t{ 1, static_cast<uint16_t>(difficulty) };
	}
}

skill_roll_result_t skill_roll(const std::size_t settler_id, game_stats_t &stats, rltk::random_number_generator &rng, const std::string skill_name, const int difficulty) {
	const int luck_component = rng.roll_dice( 1, 20 );
	const int natural_ability = get_attribute_modifier_for_skill(stats, skill_name);
	const int8_t person_skill = get_skill_modifier(stats, skill_name);
	const int total = luck_component + natural_ability + person_skill;

	std::cout << skill_name << " roll, difficulty " << difficulty << ". 1d20 = " << luck_component << ", +" << natural_ability << " (ability) + " << +person_skill << " (skill) = " << total << "\n";

	if (luck_component == 1) {
		return CRITICAL_FAIL;
	} else if (luck_component == 20) {
		gain_skill_from_success(settler_id, stats, skill_name, difficulty, rng);
		return CRITICAL_SUCCESS;
	} else if (total >= difficulty) {
		gain_skill_from_success(settler_id, stats, skill_name, difficulty, rng);
		return SUCCESS;
	} else {
		return FAIL;
	}
}