#include "string_table.hpp"
#include <unordered_map>
#include <fstream>

namespace string_tables {

    std::unordered_map<int, string_table_t> string_tables;

    void load_string_table(const int index, const std::string &filename) noexcept {
        string_table_t target;

        std::ifstream f(filename);
        std::string line;
        while (getline(f, line)) {
            if (!line.empty()) target.strings.push_back(line);
        }

        string_tables[index] = target;
    }

    string_table_t *string_table(const int index) noexcept {
        return &string_tables.at(index);
    }

}