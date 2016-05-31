#pragma once

#include <rltk.hpp>
#include "../components/components.hpp"

class settler_ai_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
	void settler_calculate_initiative(settler_ai_t &ai, game_stats_t &stats);
	void wander_randomly(position_t &pos);
};
