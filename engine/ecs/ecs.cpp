#include "ecs.h"
#include <vector>
#include <unordered_map>
#include <memory>
#include <bits/algorithmfwd.h>

using std::vector;
using std::unordered_map;
using std::unique_ptr;

namespace engine {
namespace ecs {

unordered_map<int, entity> entities;
vector<unique_ptr<base_component>> components;
unordered_map<int, int> component_handle_index;
vector<unique_ptr<base_system>> systems;


void update_component_handle_index() {
    component_handle_index.clear();
    for (auto i=0; i<components.size(); ++i) {
        component_handle_index[components[i]->handle] = i;
    }
}

void tick(const double duration_ms) {
  update_component_handle_index();
  for (unique_ptr<base_system> &system : systems) {
    system->tick(duration_ms);
  }
}


}
}
