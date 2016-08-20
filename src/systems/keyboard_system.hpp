#pragma once

#include "../components/components.hpp"
#include <rltk.hpp>

class keyboard_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
};
