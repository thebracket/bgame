#include "ecs.h"
#include "base_message.h"
#include <vector>
#include <unordered_map>
#include <memory>
#include <queue>

using std::vector;
using std::unordered_map;
using std::unique_ptr;
using std::queue;

namespace engine {
namespace ecs {

int handle_count = 0;
unordered_map<int, entity> entities;
vector<unique_ptr<base_component>> components;
unordered_map<int, int> component_handle_index;
vector<unique_ptr<base_system>> systems;
queue<base_message> mailbox;

inline int next_handle() {
    int result = handle_count;
    ++handle_count;
    return result;
}

int add_component(unique_ptr< base_component > component)
{
    int handle = next_handle();
    component->handle = handle;
    components.push_back(std::move(component));
    return handle;
}

base_component * get_component_by_handle(const int &handle) {
    auto finder = component_handle_index.find(handle);
    if (finder == component_handle_index.end()) {
	return nullptr;
    } else {
	const int index = finder->second;
	return components[index].get();
    }
}

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
