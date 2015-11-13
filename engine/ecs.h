#pragma once

#include <memory>
#include <functional>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include "entity.h"
#include "base_system.h"
#include "entity_storage.h"
#include "component_storage.h"

using std::unique_ptr;
using std::function;
using std::unordered_map;
using std::vector;
using std::fstream;
using std::string;
using engine::base_system;

namespace engine {

/*
 * Entity-Component-System class. It should be defined in config.h, since it takes template
 * parameters for the defined components. Each component is automatically given its own vector,
 * for contiguous storage (and fast traversal!).
 * 
 * Example:
 * using GAME_ECS = entity_component_system<engine::ecs::calendar_component, engine::ecs::debug_name_component>
 * 
 * Parameters: it requires a component factory function, of the form 
 * void component_factory(fstream &lbfile, const int ct)
 * (This allows for serialization without the ECS knowing about concrete component types)
 * 
 * Known issues: 
 * 2. Systems don't really belong in here, there should be a more generic game logic callback
 * 	to give the game more control over logical execution flow.
 */
template <typename component_list>
class entity_component_system {
private:
  /* Component storage, delegated to the component_storage template class. */
  ecs_detail::component_storage<component_list> components;
  
  /* Entity storage, delegated to the entity_storage template class. */
  ecs_detail::entity_storage entities;
  
  /* A list of systems, derived from base_system, that run on each tick. */
  vector<unique_ptr<base_system>> systems;
  
  /* Is the system running? Enabled by init() and disabled by done(). */
  bool running = false;
  
  /* Callback for loading entities via the save system. */
  const function<void(fstream &, const int &)> loader_callback;
  
  /* Callback for loading world constants during save-game */
  const function<void(fstream &)> load_constants;
  
  /* Callback for saving world constants during save-game */
  const function<void(fstream &)> save_constants;
  
public:
  
  /* General constructor. Takes a callback of the form void component_factory(fstream &lbfile, const int ct) to handle
   * serialization. */
  entity_component_system(const function<void(fstream &, const int &)> loader, function<void(fstream &)> world_loader, function<void(fstream &)> world_saver) 
    : loader_callback(loader), load_constants(world_loader), save_constants(world_saver) {}
  
  /* Retrieves the next available entity handle. */
  int get_next_entity_handle() {
    return entities.get_next_handle();
  }
  
  /* Add a component to the system. The system is linked to the entity provided,
   * and the entity's component count is incremented. */
  template<typename T>
  void add_component ( entity &target, T component ) {
    component.entity_id = target.handle;
    ++target.component_count;
    components.store_component(component);
  }
  
  /* Retrieves a component by its handle ID. */
  template<typename T2>
  T2 * get_component_by_handle ( const int &handle ) {
    T2 ignore;
    T2 * result = components.find_component_by_handle(handle, ignore);
    return result;
  }
  
  /* Searches components (of a type specified by the template parameter), using a callback
   * function of the form bool (T component).
   */
  template<typename T>
  vector<T *> find_components_by_func ( function<bool ( const T &c ) > matcher ) {
    return components.find_components_by_func(matcher);
  }
  
  /*
   * Retrieves a pointer to the vector of a given component type.
   */
  template<typename T>
  vector<T> * find_components_by_type() {
    T tmp;
    return components.find_components_by_type(tmp);
  }  
  
  /*
   * Adds an entity to the ECS.
   */
  void add_entity( entity &e ) {
      entities.add_entity(e);
  }
  
  /*
   * Retrieves an entity by handle. Given that entities don't hold much information,
   * this is rarely used.
   */
  entity * get_entity_by_handle(const int &handle) {
    return entities.find_by_handle(handle);
  }
  
  /*
   * Finds an entity with a given entity handle, and a known component type. If there
   * are multiple components of a given type, it returns the first one.
   */
  template<typename T>
  T * find_entity_component(const int &entity_handle) {
    T ignore;
    return components.find_entity_component(entity_handle, ignore);
  }
  
  /*
   * Adds a system to the systems vector. All systems are executed on tick().
   */
  void add_system ( unique_ptr<base_system> system ) {
    systems.push_back ( std::move ( system ) );
  }    
  
  /*
   * If running, executes all systems in the order in which they were added.
   */
  void tick ( const double duration_ms )
  {
      if (!running) return;
      // TODO: Temporary for compatibility
      for ( unique_ptr<base_system> &system : systems ) {
	  system->tick ( duration_ms );
      }
      components.clear_deleted( entities );
      entities.delete_empty_entities();
  }
  
  /*
   * Marks the entity-component system as running.
   */
  void init() {
     running = true;
  }
  
  /*
   * Marks the entity-component system as stopped.
   */
  void done() {    
    running = false;
  }     
  
  /*
   * Loads all entities and components from the save-game file.
   */
  void load_game(const string filename) {
    entities.clear_all();
    components.clear_all();
    
    fstream lbfile ( filename, std::ios::in | std::ios::binary );
    load_constants ( lbfile );
    
    int number_of_entities = 0;
    lbfile.read ( reinterpret_cast<char *> ( &number_of_entities ), sizeof ( number_of_entities ) );
    for ( int i=0; i<number_of_entities; ++i ) {
	entity e = construct_entity_from_file ( lbfile );
	add_entity ( e );
    }
    
    int number_of_components = 0;
    lbfile.read ( reinterpret_cast<char *> ( &number_of_components ), sizeof ( number_of_components ) );
    for ( int i=0; i<number_of_components; ++i ) {
      int ct;
      lbfile.read ( reinterpret_cast<char *> ( &ct ), sizeof ( ct ) );
      loader_callback(lbfile, ct);
    }
  }
  
  /*
   * Serializes all entities and components to the save-game file.
   */
  void save_game(const string filename) {
     fstream lbfile ( filename, std::ios::out | std::ios::binary );

     save_constants ( lbfile );

     int number_of_entities = entities.size();
     lbfile.write ( reinterpret_cast<const char *> ( &number_of_entities ), sizeof ( number_of_entities ) );
     entities.for_each([&lbfile] (entity * e) { e->save(lbfile); });

     int number_of_components = components.size();
     lbfile.write ( reinterpret_cast<const char *> ( &number_of_components ), sizeof ( number_of_components ) );
     components.save(lbfile);
  }
};

}
