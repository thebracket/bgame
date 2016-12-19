#pragma once

#include <rltk.hpp>
#include "../components/position.hpp"

class grazer_ai_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
    void wander_randomly(rltk::entity_t &entity, position_t &original);
};
