#pragma once

#include <rltk.hpp>
#include <vector>

extern std::vector<int> mining_map;

class mining_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
	bool dirty = true;
};
