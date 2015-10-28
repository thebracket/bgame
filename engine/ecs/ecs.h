#pragma once

#include "entity.h"
#include "systems/base_system.h"
#include "components/component_types.h"
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
#include <fstream>
#include <iostream>

#include "entity_storage.h"
#include "component_storage.h"
#include "systems/system_factory.h"
#include "serialization.h"
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
  
  template<typename T>
  void load_component_factory ( fstream &lbfile )
  {
      T component = engine::ecs::serialization::load_component<T> ( lbfile );
      const int entity_handle = component.entity_id;
      entity * parent = get_entity_by_handle ( entity_handle );
      add_component<T> ( *parent, component );
  }
  
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
      engine::ecs::component_type ct;
      lbfile.read ( reinterpret_cast<char *> ( &ct ), sizeof ( ct ) );
      
      switch ( ct ) {
      case engine::ecs::component_type::position :
	    load_component_factory<engine::ecs::position_component> ( lbfile );
	    break;
      case engine::ecs::component_type::name :
	    load_component_factory<engine::ecs::debug_name_component> ( lbfile );
	    break;
      case engine::ecs::component_type::renderable :
	    load_component_factory<engine::ecs::renderable_component> ( lbfile );
	    break;
      case engine::ecs::component_type::viewshed :
	    load_component_factory<engine::ecs::viewshed_component> ( lbfile );
	    break;
      case engine::ecs::component_type::calendar : 
	    load_component_factory<engine::ecs::calendar_component> ( lbfile );
	    break;
      case engine::ecs::component_type::settler_ai :
	    load_component_factory<engine::ecs::settler_ai_component> ( lbfile );
	    break;
      case engine::ecs::component_type::obstruction :
	    load_component_factory<engine::ecs::obstruction_component> ( lbfile );
	    break;
      case engine::ecs::component_type::power_generator :
	    load_component_factory<engine::ecs::power_generator_component> ( lbfile );
	    break;
      case engine::ecs::component_type::power_battery :
	    load_component_factory<engine::ecs::power_battery_component> ( lbfile );
	    break;
      default :
	    throw 102;
      }
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
