#include "grazer_ai.hpp"
#include "../utils/serialization_wrapper.hpp"

void grazer_ai::save(std::ostream &f) {
    Serialize("grazer_ai", f, initiative, initiative_modifier);
}

grazer_ai grazer_ai::load(std::istream &f) {
    grazer_ai c;
    Deserialize("grazer_ai", f, c.initiative, c.initiative_modifier);
    return c;
}