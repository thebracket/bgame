#pragma once

#include <rltk.hpp>

class mode_design_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
    void digging();
    void building();
    void chopping();
    void guardposts();
	void stockpiles();
	void harvest();
	void architecture();
	int selected_building = 0;
};

extern int architecture_mode;
extern int arch_width;
extern int arch_height;
extern bool arch_filled;
extern bool arch_available;
extern bool arch_possible;
extern int arch_x;
extern int arch_y;
