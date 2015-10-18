#include "ecs.h"
#include "base_message.h"
#include "system_factory.h"
#include <vector>
#include <unordered_map>
#include <memory>
#include <queue>

using std::vector;
using std::unordered_map;
using std::unique_ptr;
using std::queue;
using std::bitset;

namespace engine {
namespace ecs {

int handle_count = 0;
unordered_map<int, entity> entities;
vector<unique_ptr<base_component>> components;
unordered_map<int, int> component_handle_index;
vector<unique_ptr<base_system>> systems;
//queue<base_message> mailbox;

/*
 * Return the next unique handle for a component. When threading is introduced,
 * this will require some locking semantics - for now it's very simple.
 */
inline int next_handle() {
    int result = handle_count;
    ++handle_count;
    return result;
}

/*
 * Adds a component to the system, attached to an entity. The entity will
 * be updated to hold a handle to the component, as well as its "types"
 * bitset updated.
 */
void add_component(entity &target, unique_ptr< base_component > component)
{
    int handle = next_handle();
    component->handle = handle;
    target.component_handles.push_back(handle);
    target.component_types.set(component_flag(component->type));
    components.push_back(std::move(component));
}

/*
 * Find a component by handle, and return a non-owning pointer to it; otherwise,
 * return nullptr.
 */
base_component * get_component_by_handle(const int &handle) {
    auto finder = component_handle_index.find(handle);
    if (finder == component_handle_index.end()) {
        return nullptr;
    } else {
        const int index = finder->second;
        return components[index].get();
    }
}

/*
 * Adds an entity to the entity map
 */
void add_entity(const entity &target) {
    entities[target.handle] = target;
}

/*
 * Returns a non-owning pointer (or nullptr) to an entity
 * with a matching entity handle.
 */
entity * get_entity_by_handle(const int &handle) {
    auto finder = entities.find(handle);
    if (finder == entities.end()) return nullptr;
    return &finder->second;
}

/**
 * Returns a list of entity handles that match a given function.
 */
vector<int> find_entities_by_func(function<bool(const entity &e)> matcher) {
    vector<int> result;
    for (auto it = entities.begin(); it != entities.end(); ++it) {
	const bool match = matcher(it->second);
	if (match) result.push_back(it->second.handle);
    }
    return result;
}

/**
 * Return a list of entities with a bit set (i.e. entities with a type of component)
 */
vector<int> find_entities_by_bitset(const int &bit_to_check) {
    return find_entities_by_func([bit_to_check] (const entity &e) { 
	return e.component_types.test(bit_to_check);
    } );
}

/*
 * Returns a vector of non-owning pointers to components that match a passed in
 * matcher-function.
 */
vector<base_component *> find_components_by_func(function<bool(const base_component &c)> matcher) {
    vector<base_component *> result;
    for (const unique_ptr<base_component> &c : components) {
	if (matcher(*c)) result.push_back(c.get());
    }
    return result;
}

/*
 * Obtains a list of components matching a type specification
 */
vector<base_component *> find_components_by_type(const component_type &t) {
    return find_components_by_func([t] (const base_component &c) { 
	return (c.type == t);
    });
}

/*
 * Updates the index of component handles to actual vector positions.
 */
void update_component_handle_index() {
    component_handle_index.clear();
    for (auto i=0; i<components.size(); ++i) {
        component_handle_index[components[i]->handle] = i;
    }
}

/*
 * Adds a system to the list of systems.
 */
void add_system(unique_ptr<base_system> system) {
    systems.push_back(std::move(system));
}

/*
 * Called each frame, runs all registered systems.
 */
void tick(const double duration_ms) {
    update_component_handle_index();
    for (unique_ptr<base_system> &system : systems) {
        system->tick(duration_ms);
    }
}

inline void empty_all() {
    entities.clear();
    components.clear();
    systems.clear();
}

void init() {
    empty_all();
    add_system(make_test_system());
    add_system(make_camera_system());
}

void done() {
    empty_all();
}



}
}
