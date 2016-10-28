#pragma once

#include <rltk.hpp>
#include "../../components/position.hpp"
#include "../../components/viewshed.hpp"
#include <functional>
#include <limits>

namespace tasks {

struct spotted_hostile_t {
    bool terrified = false;
    float terror_distance = std::numeric_limits<float>::max();
    std::size_t closest_fear = 0;
};

spotted_hostile_t can_see_hostile(const rltk::entity_t &e, const position_t &pos, const viewshed_t &view, const std::function<bool(rltk::entity_t&)> &test);
}
