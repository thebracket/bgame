#pragma once

#include <rltk.hpp>

class door_system : public rltk::base_system {
public:
	virtual void configure() override final;
	virtual void update(const double duration_ms) override final;
private:
    bool dirty = true;
};
