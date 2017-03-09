#include "species.hpp"

std::string species_t::gender_str() {
    switch (gender) {
        case MALE : return "Male";
        case FEMALE : return "Female";
        default : return "Male";
    }
}

std::string species_t::gender_pronoun() {
    switch (gender) {
        case MALE : return "He";
        case FEMALE : return "She";
    }
}

std::string species_t::sexuality_str() {
    switch (sexuality) {
        case HETEROSEXUAL : return "Heterosexual";
        case HOMOSEXUAL : return "Homosexual";
        case BISEXUAL : return "Bi-sexual";
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

std::string species_t::ethnicity() {
    switch (skin_color) {
        case CAUCASIAN : return "caucasian";
        case ASIAN : return "asian";
        case INDIAN : return "middle-eastern";
        case AFRICAN : return "african";
    }
}

std::string species_t::hair_color_str() {
    switch (hair_color) {
        case WHITE_HAIR : return "white";
        case BROWN_HAIR : return "brown";
        case BLACK_HAIR : return "black";
        case BLONDE_HAIR : return "blonde";
        case RED_HAIR : return "red";
    }
}

std::string species_t::hair_style_str() {
    switch (hair_style) {
        case BALD : return "bald";
        case SHORT_HAIR : return "cropped short";
        case LONG_HAIR : return "long";
        case PIGTAILS : return "in pigtails";
        case MOHAWK : return "in a mowhawk";
        case BALDING : return "balding";
        case TRIANGLE : return "cut into a triangle shape";
    }
}
