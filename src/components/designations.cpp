#include "designations.hpp"
#include "../utils/serialization_wrapper.hpp"

void designations_t::save(std::ostream &f) {
    Serialize(f, mining, chopping, buildings, build_orders, guard_points, deconstructions, current_power, current_cash, standing_order_idle_move, standing_order_wildlife_treatment, standing_order_upgrade);
}

designations_t designations_t::load(std::istream &f) {
    designations_t c;
    Deserialize(f, c.mining, c.chopping, c.buildings, c.build_orders, c.guard_points, c.deconstructions, c.current_power, c.current_cash, c.standing_order_idle_move, c.standing_order_wildlife_treatment, c.standing_order_upgrade);
    return c;
}
