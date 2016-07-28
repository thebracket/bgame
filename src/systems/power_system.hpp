#pragma once

#include <rltk.hpp>

class power_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
	bool has_run = false;
	void calculate(int total_consumed_this_tick);
};