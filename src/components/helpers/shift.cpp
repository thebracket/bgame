#include "shift.hpp"
#include "../../utils/serialization_wrapper.hpp"

void shift_t::save(std::ostream &f) {
    for (int i=0; i<24; ++i) {
        Serialize(f, shift_name, hours[i]);
    }
}

void shift_t::load(std::istream &f) {
    for (int i=0; i<24; ++i) {
        Deserialize(f, shift_name, hours[i]);
    }
}
