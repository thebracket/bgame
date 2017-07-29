#pragma once

#include <rltk.hpp>
#include "../../components/settler_ai.hpp"
#include "../../components/position.hpp"

namespace tasks {

void drop_current_tool(const rltk::entity_t &e, settler_ai_t &ai, const position_t &pos);

}
