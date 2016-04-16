#pragma once

#include "../../engine/ecs.hpp"
#include "../components/calendar_component.h"
#include "../components/settler_ai_component.h"
#include <sstream>
#include <string>
#include "../../engine/rng.hpp"

using std::string;
using std::stringstream;
using engine::base_system;

class settler_ai_system: public base_system
{
public:
	settler_ai_system()
	{
		system_name = "Settler AI System";
	}
	virtual void tick(const double &duration_ms);
};
