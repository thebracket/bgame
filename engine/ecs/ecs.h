#pragma once

#include "entity.h"
#include "systems/base_system.h"
#include <memory>
#include <functional>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <iostream>

#include "entity_storage.h"
#include "component_storage.h"
#include "systems/system_factory.h"
#include "../../game/world/world.h"

using std::unique_ptr;
using std::function;
using std::unordered_map;
using std::vector;
using std::fstream;
using engine::ecs::base_system;

namespace engine {

using ecs::entity;
  
template <typename... component_list>
class entity_component_system {
private:
  ecs_detail::component_storage<component_list...> components;
  ecs_detail::entity_storage entities;
  vector<unique_ptr<base_system>> systems;
public:
  template<typename T>
  void add_component ( entity &target, T component ) {
    component.entity_id = target.handle;
    components.store_component(component);
  }
  
  template<typename T2>
  T2 * get_component_by_handle ( const int &handle ) {
    T2 ignore;
    T2 * result = components.find_component_by_handle(handle, ignore);
    return result;
  }
  
  template<typename T>
  vector<T *> find_components_by_func ( function<bool ( const T &c ) > matcher ) {
    return components.find_components_by_func(matcher);
  }
  
  template<typename T>
  vector<T> * find_components_by_type() {
    T tmp;
    return components.find_components_by_type(tmp);
  }
  
  int add_entity( entity &e ) {
      e.handle = entities.get_next_handle();
      entities.add_entity(e);
      return e.handle;
  }
  
  ecs::entity * get_entity_by_handle(const int &handle) {
    return entities.find_by_handle(handle);
  }
  
  template<typename T>
  T * find_entity_component(const int &entity_handle) {
    T ignore;
    return components.find_entity_component(entity_handle, ignore);
  }
  
  void add_system ( unique_ptr<base_system> system ) {
    systems.push_back ( std::move ( system ) );
  }
  
  bool running = false;
  
  void tick ( const double duration_ms )
  {
      if (!running) return;
      // TODO: Temporary for compatibility
      for ( unique_ptr<base_system> &system : systems ) {
	  system->tick ( duration_ms );
      }
  }
  
  void init() {
     // TODO: Temporary for compatibility during transition
     add_system ( engine::ecs::make_camera_system() );
     add_system ( engine::ecs::make_calendar_system() );
     add_system ( engine::ecs::make_obstruction_system() );
     add_system ( engine::ecs::make_power_system() );
     add_system ( engine::ecs::make_settler_ai_system() );
     add_system ( engine::ecs::make_renderable_system() );
     add_system ( engine::ecs::make_viewshed_system() );
     running = true;
  }
  
  void done() {    
    running = false;
  }
  
  inline string get_filename()
  {
      return "world/saved_game.dat";
  }  
  
  function<void(fstream &, const int &)> loader_callback;
  
  void load_game() {
    const string filename = get_filename();
    fstream lbfile ( filename, std::ios::in | std::ios::binary );
    world::load_world_constants ( lbfile );
    
    int number_of_entities = 0;
    lbfile.read ( reinterpret_cast<char *> ( &number_of_entities ), sizeof ( number_of_entities ) );
    for ( int i=0; i<number_of_entities; ++i ) {
	entity e = engine::ecs::construct_entity_from_file ( lbfile );
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
  
  void save_game() {
    const string filename = get_filename();
     fstream lbfile ( filename, std::ios::out | std::ios::binary );

     world::save_world_constants ( lbfile );

     int number_of_entities = entities.size();
     lbfile.write ( reinterpret_cast<const char *> ( &number_of_entities ), sizeof ( number_of_entities ) );
     entities.for_each([&lbfile] (entity * e) { e->save(lbfile); });

     int number_of_components = components.size();
     lbfile.write ( reinterpret_cast<const char *> ( &number_of_components ), sizeof ( number_of_components ) );
     components.save(lbfile);
  }
};

}
