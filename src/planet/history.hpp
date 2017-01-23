#pragma once

#include <fstream>
#include <vector>
#include <unordered_map>
#include <rltk.hpp>

struct life_event_t {
	life_event_t() {}
	life_event_t(const int &YEAR, const std::string &TYPE) : year(YEAR), type(TYPE) {}
    int year = 0;
    std::string type = "";
};

struct history_t {
    std::unordered_map<std::size_t, std::vector<life_event_t>> settler_life_events;

    void save(std::fstream &deflate) const;
    void load(std::fstream &inflate);
};
