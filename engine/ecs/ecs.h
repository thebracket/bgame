#pragma once

#include "entity.h"
#include "components/base_component.h"
#include "systems/base_system.h"
#include <memory>
#include <functional>
#include <unordered_map>
#include <vector>

using std::unique_ptr;
using std::function;
using std::unordered_map;
using std::vector;

namespace engine {
namespace ecs {

extern unordered_map<int, int> component_handle_index;
extern vector<unique_ptr<base_component>> components;
  
void init();
void done();
  

/*
 * Add a system to the processing list.
 */
void add_system(unique_ptr<base_system> system);

/*
 * Adds a component to the system, attached to an entity. The entity will
 * be updated to hold a handle to the component, as well as its "types"
 * bitset updated.
 */
void add_component(engine::ecs::entity& entity, unique_ptr< engine::ecs::base_component > component);

/*
 * Find a component by handle, and return a non-owning pointer to it; otherwise,
 * return nullptr.
 */
template<typename T> 
T * get_component_by_handle(const int &handle) {
    auto finder = component_handle_index.find(handle);
    if (finder == component_handle_index.end()) {
        return nullptr;
    } else {
        const int index = finder->second;
        return static_cast<T*>(components[index].get());
    }
}

/*
 * Adds an entity to the entity map
 */
void add_entity(const entity &target);

/*
 * Returns a non-owning pointer (or nullptr) to an entity
 * with a matching entity handle.
 */
entity * get_entity_by_handle(const int &handle);

/**
 * Returns a list of entity handles that match a given function.
 */
vector<int> find_entities_by_func(function<bool(const entity &e)> matcher);

/*
 * Returns a vector of non-owning pointers to components that match a passed in
 * matcher-function.
 */
template<typename T>
vector<T *> find_components_by_func(function<bool(const T &c)> matcher) {
    vector<T *> result;
    for (const unique_ptr<base_component> &c : components) {
	base_component * bc_ptr = c.get();
	T * bc_typed = static_cast<T *>(bc_ptr);
	if (matcher(*bc_typed)) result.push_back(bc_typed);
    }
    return result;
}

/*
 * Obtains a list of components matching a type specification
 */
template <typename T>
vector<T *> find_components_by_type(const component_type &t) {
    return find_components_by_func<T>([t] (const T &c) { 
	return (c.type == t);
    });
}

/**
 * Return a list of entities with a bit set (i.e. entities with a type of component)
 */
vector<int> find_entities_by_bitset(const int &bit_to_check);

/* The tick routine run every frame. Calls each system. */
void tick(const double duration_ms);

/* Serialization */
void load_game();
void save_game();

}
}
