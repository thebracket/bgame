#include "lightsource.hpp"
#include "../utils/serialization_wrapper.hpp"

void lightsource_t::save(std::ostream &lbfile) {
    Serialize("lightsource_t", lbfile, radius, color, alert_status);
}

lightsource_t lightsource_t::load(std::istream &lbfile) {
    lightsource_t c;
    Deserialize("lightsource_t", lbfile, c.radius, c.color, c.alert_status);
    return c;
}