#include "shift.hpp"
#include "../../utils/serialization_wrapper.hpp"

void shift_t::save(std::ostream &f) {
    Serialize("shift_t", f, shift_name);
    for (int i=0; i<24; ++i) {
        Serialize("shift_t_hr", f, hours[i]);
    }
}

void shift_t::load(std::istream &f) {
    Deserialize("shift_t", f, shift_name);
    for (int i=0; i<24; ++i) {
        Deserialize("shift_t_hr", f, hours[i]);
    }
}
