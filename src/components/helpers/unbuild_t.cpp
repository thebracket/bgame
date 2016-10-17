#include "unbuild_t.hpp"
#include "../../utils/serialization_wrapper.hpp"

void unbuild_t::save(std::ostream &f) {
    Serialize("unbuild_t", f, is_building, building_id);    
}

void unbuild_t::load(std::istream &f) {
    Deserialize("unbuild_t", f, is_building, building_id);   
}
