#include "raw_power_generator.h"
#include "../ecs/ecs.h"
#include "../ecs/components/power_generator_component.h"
#include "../globals.h"

namespace engine {
namespace raws {

void raw_power_generator::build_components(engine::ecs::entity &parent, const int &x, const int &y) const {
    engine::globals::ecs->add_component(parent, engine::ecs::power_generator_component(amount, condition));
}

}
}