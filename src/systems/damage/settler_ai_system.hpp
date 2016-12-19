#pragma once

#include <rltk.hpp>
#include "../components/settler_ai.hpp"
#include "../components/game_stats.hpp"
#include "../components/species.hpp"
#include "../components/position.hpp"
#include "../components/name.hpp"

class settler_ai_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
};
