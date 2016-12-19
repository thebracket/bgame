#pragma once

#include <rltk.hpp>

class damage_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
	void creature_attacks();
	void apply_damage();
	void inflict_death();
	void heal_over_time();
};
