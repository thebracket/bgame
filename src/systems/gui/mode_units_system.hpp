#pragma once

#include <rltk.hpp>

class mode_units_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
	int current_settler=0;
    int current_critter=0;
    int current_native=0;

    void render_settlers();
    void render_creatures();
    void render_natives();
};
