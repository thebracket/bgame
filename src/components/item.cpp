#include "item.hpp"
#include "../utils/serialization_wrapper.hpp"

void item_t::save(std::ostream &lbfile) {
    Serialize(lbfile, item_name, item_tag, category, type, material, claimed, stack_size);
}

item_t item_t::load(std::istream &lbfile) {
    item_t c;
    Deserialize(lbfile, c.item_name, c.item_tag, c.category, c.type, c.material, c.claimed, c.stack_size);
    return c;
}
