#pragma once

#include "entity.h"
#include "base_component.h"
#include "base_system.h"
#include <memory>

using std::unique_ptr;

namespace engine {
namespace ecs {

/* Register a component, returning the component's handle */  
int add_component(unique_ptr<base_component> component);

/* Find a component by handle, return a (weak - non-owning) pointer to the
 * component, or nullptr if the handle is not found.
 */
base_component * get_component_by_handle(const int &handle);

/* The tick routine run every frame. Calls each system. */
void tick(const double duration_ms);

}
}
