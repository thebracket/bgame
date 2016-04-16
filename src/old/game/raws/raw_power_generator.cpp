#include "raw_power_generator.h"
#include "../components/power_generator_component.h"
#include "../game.h"

namespace raws
{

void raw_power_generator::build_components(entity &parent, const int &x,
		const int &y, const bool placeholder) const
{
	if (!placeholder)
		ECS->add_component(parent,
				power_generator_component(amount, condition));
}

}
