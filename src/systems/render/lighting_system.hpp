#pragma once

#include <rltk.hpp>
#include <vector>

extern std::vector<rltk::color_t> light_map;
extern std::unordered_map<int, rltk::color_t> lit_tiles;

class lighting_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
	bool lighting_changed = true;
    bool dirty = true;
    double timer = 0.0;
};