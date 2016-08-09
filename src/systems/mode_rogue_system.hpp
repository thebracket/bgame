#pragma once

#include "../components/components.hpp"
#include <rltk.hpp>

class mode_rogue_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
	void settler_calculate_initiative(settler_ai_t &ai, game_stats_t &stats);
};
