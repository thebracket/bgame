#include "raw_power_generator.h"
#include "../components/power_generator_component.h"
#include "../../engine/globals.h"

namespace engine {
namespace raws {

void raw_power_generator::build_components(engine::ecs::entity &parent, const int &x, const int &y) const {
    engine::globals::ecs->add_component(parent, engine::ecs::power_generator_component(amount, condition));
}

}
}