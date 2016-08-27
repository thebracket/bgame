#include "civilizations.hpp"
#include <rltk.hpp>

using namespace rltk;

void civ_holder_t::save(std::fstream &deflate) const {
    serialize(deflate, civs.size());
    for (const auto &civ : civs) civ.save(deflate);
    serialize(deflate, unimportant_people.size());
    for (const auto &person : unimportant_people) person.save(deflate);
    serialize(deflate, settlements.size());
    for (const auto &settlement : settlements) settlement.save(deflate);
}

void civ_holder_t::load(std::fstream &inflate) {
    std::size_t sz;

    deserialize(inflate, sz);
    civs.resize(sz);
    for (std::size_t i=0; i<sz; ++i) civs[i].load(inflate);

    deserialize(inflate, sz);
    unimportant_people.resize(sz);
    for (std::size_t i=0; i<sz; ++i) unimportant_people[i].load(inflate);

    deserialize(inflate, sz);
    settlements.resize(sz);
    for (std::size_t i=0; i<sz; ++i) settlements[i].load(inflate);
}

void settlement_t::save(std::fstream &deflate) const {
    serialize(deflate, civ_id);
    serialize(deflate, name);
    serialize(deflate, world_x);
    serialize(deflate, world_y);
}

void settlement_t::load(std::fstream &inflate) {
    deserialize(inflate, civ_id);
    deserialize(inflate, name);
    deserialize(inflate, world_x);
    deserialize(inflate, world_y);
}

void civ_t::save(std::fstream &deflate) const {
    serialize(deflate, name);
    serialize(deflate, species_tag);
    serialize(deflate, relations.size());
    for (auto it = relations.begin(); it!=relations.end(); ++it) {
        serialize(deflate, it->first);
        serialize(deflate, it->second);
    }
}

void civ_t::load(std::fstream &inflate) {
    deserialize(inflate, name);
    deserialize(inflate, species_tag);
    std::size_t sz;
    deserialize(inflate, sz);
    for (std::size_t i=0; i<sz; ++i) {
        std::size_t idx;
        int feelings;
        deserialize(inflate, idx);
        deserialize(inflate, feelings);
        relations[idx] = feelings;
    }
}

void unimportant_person_t::save(std::fstream &deflate) const {
    serialize(deflate, civ_id);
    serialize(deflate, species_tag);
    serialize(deflate, world_x);
    serialize(deflate, world_y);
    serialize(deflate, male);
    serialize(deflate, married);
    serialize(deflate, married_to);
    serialize(deflate, age);
    serialize(deflate, deceased);
    serialize(deflate, occupation);
}

void unimportant_person_t::load(std::fstream &inflate) {
    deserialize(inflate, civ_id);
    deserialize(inflate, species_tag);
    deserialize(inflate, world_x);
    deserialize(inflate, world_y);
    deserialize(inflate, male);
    deserialize(inflate, married);
    deserialize(inflate, married_to);
    deserialize(inflate, age);
    deserialize(inflate, deceased);
    deserialize(inflate, occupation);
}