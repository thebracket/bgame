#pragma once

#include <fstream>
#include <vector>
#include <boost/container/flat_map.hpp>
#include <rltk.hpp>

struct life_event_t {
    int year = 0;
    std::string type = "";
};

struct history_t {
    boost::container::flat_map<std::size_t, std::vector<life_event_t>> settler_life_events;

    void save(std::fstream &deflate) const;
    void load(std::fstream &inflate);
};
