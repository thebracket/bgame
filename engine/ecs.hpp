/*
 * BracketECS : a modern C++14 Entity-Component System.
 *
 * Copyright (c) 2015, Bracket Productions. Licensed under the Academic Free License.
 *
 * Heavily inspired by Vottorio Romeo's excellent CPPCon presentation,
 * available here: https://github.com/SuperV1234/cppcon2015
 */

#ifndef BRACKET_ECS
#define BRACKET_ECS

#include <memory>
#include <functional>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>

using std::unique_ptr;
using std::function;
using std::unordered_map;
using std::vector;
using std::fstream;
using std::string;

namespace engine {

/*
 * Base class for a system that is called on each tick.
 */
class base_system {
public:
     /* Pure virtual function; every system MUST provide this. */
     virtual void tick ( const double &duration_ms ) =0;
};

/*
 * Represents an entity. Entities in the Bracket ECS are very small indeed,
 * holding only an ID number and a reference count (number of components
 * that reference it).
 *
 * Serialization is also included.
 */
class entity {
public:
     /* The unique ID number representing the entity. */
     int handle;

     /* Reads the entity ID from a file stream. */
     void load ( fstream& lbfile ) {
          lbfile.read ( reinterpret_cast<char *> ( &handle ), sizeof ( handle ) );
     }

     /* Writes the entity ID number to a file stream. */
     void save ( fstream& lbfile ) {
          lbfile.write ( reinterpret_cast<const char *> ( &handle ), sizeof ( handle ) );
     }

     /*
      * The number of components currently referencing this entity. Incremented by the
      * add_component function, decremented by the component removal function as part
      * of a normal tick.
      */
     int component_count = 0;
};

/*
 * Helper function that constructs an entity, loads its ID number,
 * and returns it.
 */
inline entity construct_entity_from_file ( fstream& lbfile )
{
     entity e;
     e.load ( lbfile );
     return e;
}


namespace ecs_detail {

/*
 * Recursive for_each system for templates.
 */
template <typename Tuple, typename F, std::size_t ...Indices>
void for_each_impl ( Tuple&& tuple, F&& f, std::index_sequence<Indices...> )
{
     using swallow = int[];
     ( void ) swallow {1,
                       ( f ( std::get<Indices> ( std::forward<Tuple> ( tuple ) ) ), void(), int{} )...
                      };
}

/*
 * Recursive for_each system for templates.
 */
template <typename Tuple, typename F>
void for_each ( Tuple&& tuple, F&& f )
{
     constexpr std::size_t N = std::tuple_size<std::remove_reference_t<Tuple>>::value;
     for_each_impl ( std::forward<Tuple> ( tuple ), std::forward<F> ( f ),
                     std::make_index_sequence<N> {} );
}

/*
 * Template class that implements a list of types, and keeps
 * track of the size of the list via constexpr.
 */
template <typename... Ts>
struct type_list {
     static constexpr std::size_t size {sizeof... ( Ts ) };
};


/*
 * Renamer (taken from Romeo's implementation).
 */
template <template <typename...> class TNewName, typename T>
struct rename_helper;

// "Renames" `TOldName<Ts...>` to `TNewName<Ts...>`.
template <template <typename...> class TNewName,
         template <typename...> class TOldName, typename... Ts>
struct rename_helper<TNewName, TOldName<Ts...>> {
     using type = TNewName<Ts...>;
};

template <template <typename...> class TNewName, typename T>
using rename = typename rename_helper<TNewName, T>::type;

/*
 * Storage of entities. They are in a map, because
 * most of the time they are referenced by handle (when
 * they are referenced at all - most of the time you
 * don't need to).
 */
class entity_storage {
private:
     /*
      * A map of entities, indexed on the entity ID #.
      */
     unordered_map<int, entity> entities;

     /*
      * Counter for selecting the next entity ID #. This could
      * be replaced with an <atomic> if you require thread safety,
      * but there would also need to be a lock_guard placed around
      * the map.
      */
     int next_handle = 1;

public:
     /*
      * How many entities are present?
      */
     std::size_t size() {
          return entities.size();
     }

     /*
      * Call a function (with signature void(entity *) on each
      * entity.
      */
     void for_each ( std::function<void ( entity * ) > func ) {
          for ( auto it=entities.begin(); it != entities.end(); ++it ) {
               func ( &it->second );
          }
     }

     /*
      * Remove all entities. Components are untouched, which an
      * lead to orphans if you aren't careful.
      */
     void clear_all() {
          entities.clear();
     }

     /*
      * Get the next entity ID #.
      */
     int get_next_handle() {
          int result = next_handle;
          ++next_handle;
          return result;
     }

     /*
      * Add an entity to the entity map. It is
      * required that the entity already have an ID #.
      */
     void add_entity ( const entity &target ) {
          entities[target.handle] = target;
     }

     /*
      * Finds an entity by its ID #/handle, and returns
      * a pointer to it.
      */
     entity * find_by_handle ( const int &handle ) {
          auto finder = entities.find ( handle );
          if ( finder != entities.end() ) {
               return &finder->second;
          }
          return nullptr;
     }

     /*
      * Iterates the entity map, removing any entities whose
      * component counts are 0 (or less, which shouldn't happen).
      */
     void delete_empty_entities() {
          auto iter = entities.begin();
          while ( iter != entities.end() ) {
               if ( iter->second.component_count < 1 ) {
                    entities.erase ( iter++ );
               } else {
                    ++iter;
               }
          }
     }
};

/*
 * Component storage system. This takes a variadic template of every component
 * type you want to use, and makes a tuple of vectors for each type. This
 * maintains contiguous storage for fast iteration, and avoids the need for an
 * inheritance hierarchy.
 *
 * Components MUST include:
 * int entity_id, handle
 * bool deleted
 * int component_type (required for serialization support).
 */
template<typename component_list>
class component_storage {
private:
     /* The next entity_id handle; this should be mad into an atomic if you
      * need multi-threading support (as well as lock_guards around insertion/deletion)
      */
     int next_handle = 1;

     /*
      * The number of component types supported by this instantiation.
      */
     static constexpr size_t number_of_component_types() {
          return component_list::type_list::size();
     }

     /*
      * A tuple of vectors, each specialized to th types described in the component
      * list specification.
      */
     template<typename ... Ts>
     using tuple_of_vectors = std::tuple<std::vector<Ts>...>;
     ecs_detail::rename<tuple_of_vectors, typename component_list::type_list> component_container;

     /*
      * Helper function that maps type T to a vector<T>, uses std::get to find the find_appropriate
      * container, and returns a pointer to it.
      */
     template <typename T>
     vector<T> * find_appropriate_bag() {
          return &std::get< vector<T> > ( component_container );
     }

public:
     /*
      * The total number of stored components.
      */
     std::size_t size() {
          std::size_t size = 0;
          for_each ( component_container, [&size] ( auto &x ) {
               size += x.size();
          } );
          return size;
     }

     /*
      * Iterate all component vectors, and delete any marked as "deleted==true".
      * Uses the remove/erase idiom.
      */
     void clear_deleted ( entity_storage &entities ) {
          for_each ( component_container, [&entities] ( auto &x ) {
               if ( !x.empty() ) {
                    auto new_end = std::remove_if ( x.begin(), x.end(), [&entities] ( auto &n ) {
                         if ( n.deleted ) {
                              entity * e = entities.find_by_handle ( n.entity_id );
                              --e->component_count;
                         }
                         return n.deleted;
                    } );
                    x.erase ( new_end, x.end() );
               }
          } );
     }

     /*
      * Clears all component vectors.
      */
     void clear_all() {
          for_each ( component_container, [] ( auto &x ) {
               x.clear();
          } );
     }

     /*
      * Gets the next available component handle.
      */
     int get_next_handle() {
          const int result = next_handle;
          ++next_handle;
          return result;
     }

     /*
      * Adds a component to its appropriate container.
      */
     template <typename T>
     int store_component ( T &component ) {
          component.handle = get_next_handle();
          find_appropriate_bag<T>()->push_back ( component );
          return component.handle;
     }

     /*
      * Finds a component by handle. Search space is reduced by
      * specifying the type (so it only has to look in that vector),
      * the search is then linear.
      */
     template <typename T>
     T * find_component_by_handle ( const int &handle_to_find, T ignore ) {
          vector<T> * storage_bag = find_appropriate_bag<T>();
          for ( T &component : *storage_bag ) {
               if ( component.handle == handle_to_find ) {
                    return &component;
               }
          }
          return nullptr;
     }

     /*
      * Iterate the vector of components of type T, calling the matcher function
      * on each. If matcher returns true, a pointer to the component is added to
      * the result vector, which is then returned.
      */
     template<typename T>
     vector<T *> find_components_by_func ( function<bool ( const T & ) > matcher ) {
          vector<T *> result;
          vector<T> * storage_bag = find_appropriate_bag<T>();
          for ( T &component : *storage_bag ) {
               if ( matcher ( component ) ) result.push_back ( &component );
          }
          return result;
     }

     /*
      * Returns a pointer to the vector of components of type T.
      */
     template<typename T>
     vector<T> * find_components_by_type ( T ignore ) {
          vector<T> * storage_bag = find_appropriate_bag<T>();
          return storage_bag;
     }

     /*
      * Find a single component of type T, belong to the entity whose
      * handle is provided.
      */
     template<typename T>
     T * find_entity_component ( const int &entity_handle, T ignore ) {
          vector<T> * storage_bag = find_appropriate_bag<T>();
          for ( T &c : *storage_bag ) {
               if ( c.entity_id == entity_handle ) return &c;
          }
          return nullptr;
     }

     /*
      * Iterates over every component, calling it's save function.
      */
     void save ( fstream &lbfile ) {
          for_each ( component_container, [&lbfile] ( auto x ) {
               for ( auto &c : x ) {
                    c.save ( lbfile );
               }
          } );
     }
};

}

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
     const function<void ( fstream &, const int & ) > loader_callback;

     /* Callback for loading world constants during save-game */
     const function<void ( fstream & ) > load_constants;

     /* Callback for saving world constants during save-game */
     const function<void ( fstream & ) > save_constants;

public:

     /* General constructor. Takes a callback of the form void component_factory(fstream &lbfile, const int ct) to handle
      * serialization. */
     entity_component_system ( const function<void ( fstream &, const int & ) > loader, function<void ( fstream & ) > world_loader, function<void ( fstream & ) > world_saver )
          : loader_callback ( loader ), load_constants ( world_loader ), save_constants ( world_saver ) {}

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
          components.store_component ( component );
     }

     /* Retrieves a component by its handle ID. */
     template<typename T2>
     T2 * get_component_by_handle ( const int &handle ) {
          T2 ignore;
          T2 * result = components.find_component_by_handle ( handle, ignore );
          return result;
     }

     /* Searches components (of a type specified by the template parameter), using a callback
      * function of the form bool (T component).
      */
     template<typename T>
     vector<T *> find_components_by_func ( function<bool ( const T & ) > matcher ) {
          return components.find_components_by_func<T> ( matcher );
     }

     /*
      * Retrieves a pointer to the vector of a given component type.
      */
     template<typename T>
     vector<T> * find_components_by_type() {
          T tmp;
          return components.find_components_by_type ( tmp );
     }

     /*
      * Adds an entity to the ECS.
      */
     void add_entity ( entity &e ) {
          entities.add_entity ( e );
     }

     /*
      * Retrieves an entity by handle. Given that entities don't hold much information,
      * this is rarely used.
      */
     entity * get_entity_by_handle ( const int &handle ) {
          return entities.find_by_handle ( handle );
     }

     /*
      * Finds an entity with a given entity handle, and a known component type. If there
      * are multiple components of a given type, it returns the first one.
      */
     template<typename T>
     T * find_entity_component ( const int &entity_handle ) {
          T ignore;
          return components.find_entity_component ( entity_handle, ignore );
     }

     template<typename T>
     vector<T *> find_entity_components ( const int &entity_handle ) {
          return find_components_by_func<T> ( [entity_handle] ( const T &e ) {
               return e.entity_id == entity_handle;
          } );
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
     void tick ( const double duration_ms ) {
          if ( !running ) return;
          // TODO: Temporary for compatibility
          for ( unique_ptr<base_system> &system : systems ) {
               system->tick ( duration_ms );
          }
          components.clear_deleted ( entities );
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
     void load_game ( const string filename ) {
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
               loader_callback ( lbfile, ct );
          }
     }

     /*
      * Serializes all entities and components to the save-game file.
      */
     void save_game ( const string filename ) {
          fstream lbfile ( filename, std::ios::out | std::ios::binary );

          save_constants ( lbfile );

          int number_of_entities = entities.size();
          lbfile.write ( reinterpret_cast<const char *> ( &number_of_entities ), sizeof ( number_of_entities ) );
          entities.for_each ( [&lbfile] ( entity * e ) {
               e->save ( lbfile );
          } );

          int number_of_components = components.size();
          lbfile.write ( reinterpret_cast<const char *> ( &number_of_components ), sizeof ( number_of_components ) );
          components.save ( lbfile );
     }
};

using engine::ecs_detail::for_each;
using engine::ecs_detail::type_list;

/*
 * Similar to the ECS, the message_bus is instantiated with a list of classes.
 * It is assumed that messages are short lived, and will not be serialized.
 * Any class that acts as a message must implement: bool deleted = false; int ttl = 32;
 */
template<typename message_list>
class message_bus {
private:
     /*
      * Tuple of vectors, just like the ECS above.
      */
     template<typename ... Ts>
     using tuple_of_vectors = std::tuple<std::vector<Ts>...>;
     ecs_detail::rename<tuple_of_vectors, typename message_list::type_list> storage;

     /*
      * Helper function to select the right vector.
      */
     template <typename T>
     vector<T> * find_appropriate_bag() {
          return &std::get< vector<T> > ( storage );
     }

public:
     /*
      * Default constructor
      */
     message_bus() {
     }

     /*
      * Iterate through all message containers, and remove any message marked as 'deleted'.
      * TTL is reduced by one, and the message deleted if it as reached zero.
      */
     void clear() {
          for_each ( storage, [] ( auto &x ) {
               if ( !x.empty() ) {
                    std::for_each ( x.begin(), x.end(), [] ( auto &n ) {
                         --n.ttl;
                         if ( n.ttl <= 0 ) n.deleted = true;
                    } );
                    auto new_end = std::remove_if ( x.begin(), x.end(), [] ( auto &n ) {
                         return n.deleted;
                    } );
                    x.erase ( new_end, x.end() );
               }
          } );
     }

     /*
      * Adds a message to the message bus.
      */
     template<typename T>
     void add_message ( T new_message ) {
          vector<T> * bag = find_appropriate_bag<T>();
          bag->push_back ( new_message );
     }

     /*
      * Retrieves a pointer to the vector of messages of type T.
      */
     template<typename T>
     vector<T> * get_messages_by_type() {
          return find_appropriate_bag<T>();
     }
};

}

#endif