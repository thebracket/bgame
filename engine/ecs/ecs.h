#pragma once

#include "entity.h"
#include "systems/base_system.h"
#include "components/component_types.h"
#include "component_bag.h"
#include "components/calendar_component.h"
#include "components/debug_name_component.h"
#include "components/obstruction_component.h"
#include "components/position_component.h"
#include "components/power_battery_component.h"
#include "components/power_generator_component.h"
#include "components/renderable_component.h"
#include "components/settler_ai_component.h"
#include "components/viewshed_component.h"
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

namespace storage {

extern int handle_count;
extern component_bag<calendar_component> calendars;
extern component_bag<debug_name_component> names;
extern component_bag<obstruction_component> obstructions;
extern component_bag<position_component> positions;
extern component_bag<power_battery_component> batteries;
extern component_bag<power_generator_component> generators;
extern component_bag<renderable_component> renderables;
extern component_bag<settler_ai_component> settler_ais;
extern component_bag<viewshed_component> viewsheds;

inline int next_handle()
{
     int result = handle_count;
     ++handle_count;
     return result;
}

}

/* COMPONENT HANDLERS */

namespace detail {
template<typename T>
void setup_component_with_entity ( entity &target, T &component )
{
     const int handle = storage::next_handle();
     component.handle = handle;
     component.entity_id = target.handle;
     target.component_handles.push_back ( std::make_pair ( component.type, handle ) );
     target.component_types.set ( component.type );
}

template<typename T>
component_bag<T> * find_appropriate_bag()
{
     throw 101;
}
template<> inline component_bag<calendar_component> * find_appropriate_bag()
{
     return &storage::calendars;
}
template<> inline component_bag<debug_name_component> * find_appropriate_bag()
{
     return &storage::names;
}
template<> inline component_bag<obstruction_component> * find_appropriate_bag()
{
     return &storage::obstructions;
}
template<> inline component_bag<position_component> * find_appropriate_bag()
{
     return &storage::positions;
}
template<> inline component_bag<power_battery_component> * find_appropriate_bag()
{
     return &storage::batteries;
}
template<> inline component_bag<power_generator_component> * find_appropriate_bag()
{
     return &storage::generators;
}
template<> inline component_bag<renderable_component> * find_appropriate_bag()
{
     return &storage::renderables;
}
template<> inline component_bag<settler_ai_component> * find_appropriate_bag()
{
     return &storage::settler_ais;
}
template<> inline component_bag<viewshed_component> * find_appropriate_bag()
{
     return &storage::viewsheds;
}

}

/*
 * Public interface to add a component to the Entity-Component-System. Links the component to the entity,
 * notifies the entity of its presence, and adds it to the appropriate storage bag.
 */
template<typename T>
void add_component ( entity &target, T component )
{
     detail::setup_component_with_entity<T> ( target, component );
     component_bag<T> * bag = detail::find_appropriate_bag<T>();
     bag->add ( component );
}

/*
 * Public interface to find a component by handle number.
 */
template<typename T>
T * get_component_by_handle ( const int &handle )
{
     component_bag<T> * bag = detail::find_appropriate_bag<T>();
     return bag->find_by_handle ( handle );
}

/*
 * Public interface to match components by lambda/function.
 */
template<typename T>
vector<T *> find_components_by_func ( function<bool ( const T &c ) > matcher )
{
     component_bag<T> * bag = detail::find_appropriate_bag<T>();
     return bag->find_by_func ( matcher );
}

/*
 * Public interface to find components by type
 */
template <typename T>
vector<T *> find_components_by_type ( const component_type &t )
{
     component_bag<T> * bag = detail::find_appropriate_bag<T>();
     return bag->find_by_type ( t );
}

/* Global Initialization */

void init();
void done();


/*
 * Add a system to the processing list.
 */
void add_system ( unique_ptr<base_system> system );

/*
 * Adds an entity to the entity map
 */
void add_entity ( const entity &target );

/*
 * Returns a non-owning pointer (or nullptr) to an entity
 * with a matching entity handle.
 */
entity * get_entity_by_handle ( const int &handle );

/**
 * Returns a list of entity handles that match a given function.
 */
vector<int> find_entities_by_func ( function<bool ( const entity &e ) > matcher );

/**
 * Return a list of entities with a bit set (i.e. entities with a type of component)
 */
vector<int> find_entities_by_bitset ( const int &bit_to_check );

/* The tick routine run every frame. Calls each system. */
void tick ( const double duration_ms );

/* Serialization */
void load_game();
void save_game();

}
}
