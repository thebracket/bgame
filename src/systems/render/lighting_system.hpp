#pragma once

#include <rltk.hpp>
#include <vector>
#include <boost/container/flat_map.hpp>

/*
 * Describes a tile as having a lightsource. Stores information about where the light is (and its nature),
 * and the relative distance to the light. The distance is used to ensure that the closest light applies.
 */
struct lit_tile_t {
    int light_position;
    rltk::color_t light_color;
    int light_distance_squared;
};

/*
 * Storage of which tiles are lit.
 */
extern boost::container::flat_map<int, lit_tile_t> lit_tiles;

/*
 * The lighting system. Responsible for determining which tiles are lit up.
 */
class lighting_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
    bool dirty = true;
};