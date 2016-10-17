#include "world_position.hpp"
#include "../utils/serialization_wrapper.hpp"

void world_position_t::save(std::ostream &lbfile) {
    Serialize("world_position", lbfile, world_x, world_y, region_x, region_y, region_z);
}

world_position_t world_position_t::load(std::istream &lbfile) {
    world_position_t c;
    Deserialize("world_position", lbfile, c.world_x, c.world_y, c.region_x, c.region_y, c.region_z);
    return c;
}
