#pragma once

#include <rltk.hpp>
#include <vector>

extern std::vector<uint8_t> mining_map;
extern std::vector<int> mining_targets;

class mining_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
	bool dirty = true;
};
