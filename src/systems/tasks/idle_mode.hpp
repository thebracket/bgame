#pragma once

#include <rltk.hpp>
#include "../../components/settler_ai.hpp"
#include "../../components/name.hpp"

namespace tasks {

void become_idle(entity_t &e, settler_ai_t &ai, name_t &name);

}