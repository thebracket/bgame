#pragma once

#include <rltk.hpp>

struct perform_construction_message : public rltk::base_message_t {
    perform_construction_message() {}
    perform_construction_message(const std::size_t &entity, const std::string &t, const std::size_t &mat) 
        : entity_id(entity), tag(t), material(mat) {}
    
    std::size_t entity_id;
    std::string tag;
    std::size_t material;
};
