#include "ecs.h"
#include "base_message.h"
#include "systems/system_factory.h"
#include "entity_factory.h"
#include "../../game/world/world.h"
#include "serialization.h"
#include <memory>
#include <queue>
#include <string>
#include <iostream>

using std::vector;
using std::unordered_map;
using std::unique_ptr;
using std::queue;
using std::bitset;
using std::string;

namespace engine {
namespace ecs {

namespace storage {
int handle_count = 0;
unordered_map<int, entity> entities;
vector<unique_ptr<base_system>> systems;
//queue<base_message> mailbox;

// Component Storage Implementations
component_bag<calendar_component> calendars;
component_bag<debug_name_component> names;
component_bag<obstruction_component> obstructions;
component_bag<position_component> positions;
component_bag<power_battery_component> batteries;
component_bag<power_generator_component> generators;
component_bag<renderable_component> renderables;
component_bag<settler_ai_component> settler_ais;
component_bag<viewshed_component> viewsheds;

inline void clear_component_bags()
{
     calendars.clear();
     names.clear();
     obstructions.clear();
     positions.clear();
     batteries.clear();
     generators.clear();
     renderables.clear();
     settler_ais.clear();
     viewsheds.clear();
}

inline unsigned int components_size()
{
     return calendars.size() +
            names.size() +
            obstructions.size() +
            positions.size() +
            batteries.size() +
            generators.size() +
            renderables.size() +
            settler_ais.size() +
            viewsheds.size();
}

}

/*
 * Adds an entity to the entity map
 */
void add_entity ( const entity &target )
{
     storage::entities[target.handle] = target;
}

/*
 * Returns a non-owning pointer (or nullptr) to an entity
 * with a matching entity handle.
 */
entity * get_entity_by_handle ( const int &handle )
{
     auto finder = storage::entities.find ( handle );
     if ( finder == storage::entities.end() ) return nullptr;
     return &finder->second;
}

/**
 * Returns a list of entity handles that match a given function.
 */
vector<int> find_entities_by_func ( function<bool ( const entity &e ) > matcher )
{
     vector<int> result;
     for ( auto it = storage::entities.begin(); it != storage::entities.end(); ++it ) {
          const bool match = matcher ( it->second );
          if ( match ) result.push_back ( it->second.handle );
     }
     return result;
}

/**
 * Return a list of entities with a bit set (i.e. entities with a type of component)
 */
vector<int> find_entities_by_bitset ( const int &bit_to_check )
{
     return find_entities_by_func ( [bit_to_check] ( const entity &e ) {
          return e.component_types.test ( bit_to_check );
     } );
}

/*
 * Adds a system to the list of systems.
 */
void add_system ( unique_ptr<base_system> system )
{
     storage::systems.push_back ( std::move ( system ) );
}

/*
 * Called each frame, runs all registered systems.
 */
void tick ( const double duration_ms )
{
     for ( unique_ptr<base_system> &system : storage::systems ) {
          system->tick ( duration_ms );
     }
}

inline void empty_all()
{
     storage::entities.clear();
     storage::systems.clear();
     storage::clear_component_bags();
}

void init()
{
     empty_all();
     add_system ( make_camera_system() );
     add_system ( make_calendar_system() );
     add_system ( make_obstruction_system() );
     add_system ( make_power_system() );
     add_system ( make_settler_ai_system() );
     add_system ( make_renderable_system() );
     add_system ( make_viewshed_system() );
}

void done()
{
     empty_all();
}

inline string get_filename()
{
     return "world/saved_game.dat";
}

template<typename T>
void load_component_factory ( fstream &lbfile )
{
     T component = serialization::load_component<T> ( lbfile );
     const int entity_handle = component.entity_id;
     entity * parent = get_entity_by_handle ( entity_handle );
     if ( parent == nullptr ) {
          std::cout << "Null parent!\n";
          std::cout << "Attempted ID: " << entity_handle << "\n";
          throw 103;
     }
     add_component<T> ( *parent, component );
}

void load_game()
{
     const string filename = get_filename();
     fstream lbfile ( filename, std::ios::in | std::ios::binary );

     world::load_world_constants ( lbfile );

     int number_of_entities = 0;
     lbfile.read ( reinterpret_cast<char *> ( &number_of_entities ), sizeof ( number_of_entities ) );
     for ( int i=0; i<number_of_entities; ++i ) {
          entity e = construct_entity_from_file ( lbfile );
          add_entity ( e );
     }

     int number_of_components = 0;
     lbfile.read ( reinterpret_cast<char *> ( &number_of_components ), sizeof ( number_of_components ) );
     //std::cout << "# Components to load: " << number_of_components << "\n";
     for ( int i=0; i<number_of_components; ++i ) {
          component_type ct;
          lbfile.read ( reinterpret_cast<char *> ( &ct ), sizeof ( ct ) );
          //std::cout << "Component " << i << " of " << number_of_components << ", typeid: " << ct << "\n";
          switch ( ct ) {
          case position :
               load_component_factory<position_component> ( lbfile );
               break;
          case name :
               load_component_factory<debug_name_component> ( lbfile );
               break;
          case renderable :
               load_component_factory<renderable_component> ( lbfile );
               break;
          case viewshed :
               load_component_factory<viewshed_component> ( lbfile );
               break;
          case calendar :
               load_component_factory<calendar_component> ( lbfile );
               break;
          case settler_ai :
               load_component_factory<settler_ai_component> ( lbfile );
               break;
          case obstruction :
               load_component_factory<obstruction_component> ( lbfile );
               break;
          case power_generator :
               load_component_factory<power_generator_component> ( lbfile );
               break;
          case power_battery :
               load_component_factory<power_battery_component> ( lbfile );
               break;
          default :
               throw 102;
          }
     }
}

void save_game()
{
     const string filename = get_filename();
     fstream lbfile ( filename, std::ios::out | std::ios::binary );

     world::save_world_constants ( lbfile );

     int number_of_entities = storage::entities.size();
     lbfile.write ( reinterpret_cast<const char *> ( &number_of_entities ), sizeof ( number_of_entities ) );
     for ( auto it = storage::entities.begin(); it != storage::entities.end(); ++it ) {
          entity e = it->second;
          e.save ( lbfile );
     }

     int number_of_components = storage::components_size();
     lbfile.write ( reinterpret_cast<const char *> ( &number_of_components ), sizeof ( number_of_components ) );

     storage::calendars.map ( [&lbfile] ( const calendar_component * c ) {
          serialization::save_component<calendar_component> ( lbfile, *c );
     } );
     storage::names.map ( [&lbfile] ( const debug_name_component * c ) {
          serialization::save_component<debug_name_component> ( lbfile, *c );
     } );
     storage::obstructions.map ( [&lbfile] ( const obstruction_component * c ) {
          serialization::save_component<obstruction_component> ( lbfile, *c );
     } );
     storage::positions.map ( [&lbfile] ( const position_component * c ) {
          serialization::save_component<position_component> ( lbfile, *c );
     } );
     storage::batteries.map ( [&lbfile] ( const power_battery_component * c ) {
          serialization::save_component<power_battery_component> ( lbfile, *c );
     } );
     storage::generators.map ( [&lbfile] ( const power_generator_component * c ) {
          serialization::save_component<power_generator_component> ( lbfile, *c );
     } );
     storage::renderables.map ( [&lbfile] ( const renderable_component * c ) {
          serialization::save_component<renderable_component> ( lbfile, *c );
     } );
     storage::settler_ais.map ( [&lbfile] ( const settler_ai_component * c ) {
          serialization::save_component<settler_ai_component> ( lbfile, *c );
     } );
     storage::viewsheds.map ( [&lbfile] ( const viewshed_component * c ) {
          serialization::save_component<viewshed_component> ( lbfile, *c );
     } );
}



}
}
