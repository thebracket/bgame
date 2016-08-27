#pragma once

#include <fstream>

struct history_t {
    void save(std::fstream &deflate) const;
    void load(std::fstream &inflate);
};
