#include "string_table.hpp"

string_table_t first_names_male;
string_table_t first_names_female;
string_table_t last_names;
string_table_t new_arrival_quips;

void load_string_table(const std::string filename, string_table_t &target) {
    std::ifstream f(filename);
    std::string line;
    while (getline(f, line))
    {
        if (!line.empty()) target.strings.push_back(line);
    }
}
