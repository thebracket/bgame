#include "stdafx.h"
#include "logger.hpp"

LOG * LOG::text(const std::string &txt) {
    for (std::size_t i=0; i<txt.size(); ++i) {
        chars.push_back(xp::vchar{ (uint16_t)txt[i], fg, bengine::color_t{1.0f, 1.0f, 1.0f} });
    }
    return this;
}

inline name_t * get_name_component(const int &entity_id) {
    auto e = bengine::entity(entity_id);
    if (e) {
        const auto name = e->component<name_t>();
        if (name) {
            return name;
        }
    }
    return nullptr;
}

inline std::string log_display_name(const int &entity_id) {
    const auto name_comp = get_name_component(entity_id);
    if (name_comp) {
        return name_comp->first_name + std::string(" ") + name_comp->last_name;
    }
    return "Nameless Entity";
}

LOG * LOG::settler_name(const int &entity_id) {
    col(bengine::color_t{0.0f, 1.0f, 0.0f});
    text(log_display_name(entity_id));
    col(bengine::color_t{1.0f, 1.0f, 1.0f});
    return this;
}

LOG * LOG::settler_name_t(const name_t &name) {
    col(bengine::color_t{0.0f, 1.0f, 0.0f});
    text(name.first_name + std::string(" ") + name.last_name);
    col(bengine::color_t{1.0f, 1.0f, 1.0f});
    return this;
}

LOG * LOG::sentient_name(const int &entity_id) {
    col(bengine::color_t{1.0f, 1.0f, 0.0f});
    text(log_display_name(entity_id));
    col(bengine::color_t{1.0f, 1.0f, 1.0f});
    return this;
}

LOG * LOG::other_name(const int &entity_id) {
    col(bengine::color_t{1.0f, 0.0f, 1.0f});
    text(log_display_name(entity_id));
    col(bengine::color_t{1.0f, 1.0f, 1.0f});
    return this;
}

LOG * LOG::civ_name(const std::size_t &civ_id) {
/*    col(rltk::colors::CYAN);
    text(planet.civs.civs[civ_id].name);
    col(rltk::colors::WHITE);*/
    return this;
}