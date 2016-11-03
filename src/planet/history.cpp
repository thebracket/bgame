#include "history.hpp"

using namespace rltk;

void history_t::save(std::fstream &lbfile) const {
    std::size_t sz = settler_life_events.size();
    serialize(lbfile, sz);
    for (auto it=settler_life_events.begin(); it!=settler_life_events.end(); ++it) {
        serialize(lbfile, it->first);
        serialize(lbfile, it->second.size());
        for (std::size_t i=0; i<it->second.size(); ++i) {
            serialize(lbfile, it->second[i].year);
            serialize(lbfile, it->second[i].type);
        }
    }
}

void history_t::load(std::fstream &lbfile) {
    settler_life_events.clear();

    std::size_t sz;
    deserialize(lbfile, sz);
    for (std::size_t i=0; i<sz; ++i) {
        std::size_t tag;
        deserialize(lbfile, tag);
        std::size_t num_events;
        deserialize(lbfile, num_events);
        settler_life_events[i] = std::vector<life_event_t>();
        for (std::size_t j=0; j<num_events; ++j) {
            life_event_t le;
            deserialize(lbfile, le.year);
            deserialize(lbfile, le.type);
            settler_life_events[tag].push_back(le);
        }
    }
}