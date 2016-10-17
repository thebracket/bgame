#include "logger.hpp"
#include "../main/game_globals.hpp"
#include "../utils/serialization_wrapper.hpp"

using rltk::serialize;
using rltk::deserialize;

void log_line_t::save(std::ostream &f) {
    Serialize("log_line_t", f, age, chars);
}

void log_line_t::load(std::istream &f) {
    Deserialize("log_line_t", f, age, chars);
}

void logger_t::save(std::ostream &lbfile) {
    Serialize("logger_t", lbfile, lines);
}

logger_t logger_t::load(std::istream &lbfile) {
    logger_t c;
    Deserialize("logger_t", lbfile, c.lines);
    return c;
}

LOG * LOG::text(const std::string &txt) {
    for (std::size_t i=0; i<txt.size(); ++i) {
        chars.push_back(rltk::vchar{ txt[i], fg, rltk::colors::BLACK });
    }
    return this;
}

inline boost::optional<name_t &> get_name_component(const std::size_t &entity_id) {
    auto e = entity(entity_id);
    if (e) {
        auto name = e->component<name_t>();
        if (name) {
            return name;
        }
    }
    return boost::optional<name_t &>();
}

inline std::string log_display_name(const std::size_t &entity_id) {
    auto name_comp = get_name_component(entity_id);
    if (name_comp) {
        return name_comp->first_name + std::string(" ") + name_comp->last_name;
    }
    return "Nameless Entity";
}

LOG * LOG::settler_name(const std::size_t &entity_id) {
    col(rltk::colors::GREEN);
    text(log_display_name(entity_id));
    col(rltk::colors::WHITE);
    return this;
}

LOG * LOG::settler_name_t(const name_t &name) {
    col(rltk::colors::GREEN);
    text(name.first_name + std::string(" ") + name.last_name);
    col(rltk::colors::WHITE);
    return this;
}

LOG * LOG::sentient_name(const std::size_t &entity_id) {
    col(rltk::colors::YELLOW);
    text(log_display_name(entity_id));
    col(rltk::colors::WHITE);
    return this;
}

LOG * LOG::other_name(const std::size_t &entity_id) {
    col(rltk::colors::MAGENTA);
    text(log_display_name(entity_id));
    col(rltk::colors::WHITE);
    return this;
}

LOG * LOG::civ_name(const std::size_t &civ_id) {
    col(rltk::colors::CYAN);
    text(planet.civs.civs[civ_id].name);
    col(rltk::colors::WHITE);
    return this;
}