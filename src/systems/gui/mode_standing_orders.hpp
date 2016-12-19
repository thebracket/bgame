#pragma once

#include <rltk.hpp>

class mode_standing_orders : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
};
