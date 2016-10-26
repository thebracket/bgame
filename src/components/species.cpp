#include "species.hpp"
#include "../utils/serialization_wrapper.hpp"

void species_t::save(std::ostream &lbfile) {
    Serialize("species_t", lbfile, gender, sexuality, hair_color, hair_style, skin_color, height_cm, weight_kg, bearded, tag);
}

species_t species_t::load(std::istream &lbfile) {
    species_t c;
    Deserialize("species_t", lbfile, c.gender, c.sexuality, c.hair_color, c.hair_style, c.skin_color, c.height_cm, c.weight_kg, c.bearded, c.tag);
    return c;
}

std::string species_t::gender_str() {
    switch (gender) {
        case MALE : return "Male";
        case FEMALE : return "Female";
        case HERMAPHRODITE : return "Hermaphrodite";
        default : return "Male";
    }
}

std::string species_t::gender_pronoun() {
    switch (gender) {
        case MALE : return "He";
        case FEMALE : return "She";
        case HERMAPHRODITE : return "Hir";
    }
}

std::string species_t::sexuality_str() {
    switch (sexuality) {
        case HETEROSEXUAL : return "Heterosexual";
        case HOMOSEXUAL : return "Homosexual";
        case BISEXUAL : return "Bi-sexual";
        case ASEXUAL : return "A-sexual";
    }
}

std::string species_t::height_feet() {
    const float height_inches = height_cm/2.5F;
    const int height_feet = (int)height_inches/12;
    const int height_additional_inches = height_feet % 12;
    std::stringstream result;
    result << height_feet << "'" << height_additional_inches << "\"";
    return result.str();
}

std::string species_t::weight_lbs() {
    const float weight_lbs = weight_kg * 2.20462F;
    std::stringstream result;
    result.precision(0);
    result << std::fixed << weight_lbs << " lbs";
    return result.str();
}