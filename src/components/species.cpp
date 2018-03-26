#include "stdafx.h"
#include "species.hpp"
#include <cctype>
#include <clocale>
#include <sstream>
#include <fmt/format.h>

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
		default: return "Xi";
    }
}

std::string species_t::sexuality_str() {
    switch (sexuality) {
        case HETEROSEXUAL : return "Heterosexual";
        case HOMOSEXUAL : return "Homosexual";
        case BISEXUAL : return "Bi-sexual";
		default: return "Unknown";
    }
}

std::string species_t::height_feet() {
    const float height_inches = height_cm/2.5F;
    const int height_feet = (int)height_inches/12;
    const int height_additional_inches = height_feet % 12;
    fmt::MemoryWriter result;
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
    return skin_color.first;
}

std::string species_t::hair_color_str() {
    const std::string base = hair_color.first;
    std::string result = "";

    for (std::size_t i=0; i<base.size(); ++i) {
        if (std::isupper(base[i]) && i != 0) {
            result += " ";
            result += base[i];
        } else {
            result += base[i];
        }
    }

    return result;
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
		default: return "unknown";
    }
}
