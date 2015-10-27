#include "raw_power_generator.h"
#include "../ecs/power_generator_component.h"

namespace engine {
namespace raws {

void raw_power_generator::build_components(engine::ecs::entity &parent, const int &x, const int &y) const {
    unique_ptr<ecs::power_generator_component> generator = make_unique<ecs::power_generator_component>(amount, condition);
    engine::ecs::add_component(parent, std::move(generator));
}

}
}