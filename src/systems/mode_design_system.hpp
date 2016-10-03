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
};