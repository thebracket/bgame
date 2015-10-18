#pragma once

#include "entity.h"
#include "base_component.h"
#include "base_system.h"
#include <memory>
#include <functional>

using std::unique_ptr;
using std::function;

namespace engine {
namespace ecs {

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

/* Find a component by handle, return a (weak - non-owning) pointer to the
 * component, or nullptr if the handle is not found.
 */
base_component * get_component_by_handle(const int &handle);

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
vector<base_component *> find_components_by_func(function<bool(const base_component &c)> matcher);

/**
 * Return a list of entities with a bit set (i.e. entities with a type of component)
 */
vector<int> find_entities_by_bitset(const int &bit_to_check);

/*
 * Obtains a list of components matching a type specification
 */
vector<base_component *> find_components_by_type(const component_type &t);

/* The tick routine run every frame. Calls each system. */
void tick(const double duration_ms);

}
}
