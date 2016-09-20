#pragma once

#include <rltk.hpp>
#include "../components/components.hpp"

class settler_ai_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
	void settler_calculate_initiative(settler_ai_t &ai, game_stats_t &stats);
	void move_to(entity_t &e, position_t &pos, position_t &destination);
	void wander_randomly(entity_t &entity, position_t &pos);

	bool butcher_exist() const;
	bool butcher_and_corpses_exist() const;

	void change_settler_glyph(entity_t &entity, const vchar &render_as);
	void become_idle(entity_t &e, settler_ai_t &ai, name_t &name);
	void drop_current_tool(const entity_t &e, settler_ai_t &ai, const position_t &pos);
	void cancel_action(entity_t &entity, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name, const std::string reason);

	void do_sleep_time(entity_t &entity, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name);
	void do_leisure_time(entity_t &entity, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name);
	void do_work_time(entity_t &entity, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name);

	void change_job_status(settler_ai_t &ai, name_t &name, const std::string new_status);

	void do_mining(entity_t &entity, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name);
	void do_chopping(entity_t &entity, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name);
	void do_building(entity_t &entity, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name);
	void do_reaction(entity_t &entity, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name);
	void do_equip_melee(entity_t &entity, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name);
	void do_equip_ranged(entity_t &entity, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name);
	void do_equip_ammo(entity_t &entity, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name);
	void do_equip_armor(entity_t &entity, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name);
	void do_hunting(entity_t &entity, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name);
	void do_butchering(entity_t &entity, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name);
	void do_guard_duty(entity_t &entity, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name);
};
