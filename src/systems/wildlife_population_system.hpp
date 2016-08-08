#pragma once

#include <rltk.hpp>
#include "../components/position.hpp"

class wildlife_population_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
    bool first_run = true;
    void count_wildlife_populations();
    void spawn_wildlife();
    void wander_randomly(entity_t &entity, position_t &original);
    void move_to(entity_t &e, position_t &pos, position_t &destination);
};
