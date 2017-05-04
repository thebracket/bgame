#pragma once

#include <string>
#include <vector>
#include <rltk.hpp>

struct string_table_t {
    std::vector<std::string> strings;

    inline std::string random_entry(rltk::random_number_generator rng=rltk::random_number_generator()) {
        const int position = rng.roll_dice(1, static_cast<int>(strings.size())) - 1;
        return strings[position];
    }
};

extern string_table_t first_names_male;
extern string_table_t first_names_female;
extern string_table_t last_names;
extern string_table_t new_arrival_quips;
extern string_table_t menu_subtitles;

void load_string_table(const std::string filename, string_table_t &target);
