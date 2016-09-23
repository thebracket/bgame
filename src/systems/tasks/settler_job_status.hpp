#pragma once

#include <rltk.hpp>
#include "../../components/settler_ai.hpp"
#include "../../components/name.hpp"
#include <string>

namespace tasks {

void change_job_status(settler_ai_t &ai, name_t &name, const std::string new_status);

}
