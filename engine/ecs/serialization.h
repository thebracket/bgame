#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include "../virtual_terminal.h"
#include "components/calendar_component.h"
#include "components/debug_name_component.h"
#include "components/obstruction_component.h"
#include "components/position_component.h"
#include "components/power_battery_component.h"
#include "components/power_generator_component.h"
#include "components/renderable_component.h"
#include "components/settler_ai_component.h"
#include "components/viewshed_component.h"

using std::fstream;
using std::string;
using engine::vterm::color_t;

namespace engine {
namespace ecs {
namespace serialization {

/* Primitive save/load handler */

template<typename T>
inline void save_primitive ( fstream &lbfile, const T &target )
{
     lbfile.write ( reinterpret_cast<const char *> ( &target ), sizeof ( target ) );
}
template<>
inline void save_primitive ( fstream &lbfile, const string &s )
{
     unsigned int size = s.size();
     save_primitive<unsigned int> ( lbfile, size );
     for ( unsigned int i=0; i<size; ++i ) {
          save_primitive<char> ( lbfile, s[i] );
     }
}
template<>
inline void save_primitive ( fstream &lbfile, const color_t &c )
{
     unsigned char red = std::get<0> ( c );
     unsigned char green = std::get<1> ( c );
     unsigned char blue = std::get<2> ( c );
     save_primitive<unsigned char> ( lbfile, red );
     save_primitive<unsigned char> ( lbfile, green );
     save_primitive<unsigned char> ( lbfile, blue );
}
template<>
inline void save_primitive ( fstream &lbfile, const bool &b )
{
     int n = 0;
     if ( b ) n=1;
     save_primitive<int> ( lbfile,n );
}
template<>
inline void save_primitive ( fstream &lbfile, const component_type &t )
{
     int n = t;
     save_primitive<int> ( lbfile, n );
}

template <typename T>
inline void load_primitive ( fstream &lbfile, T &target )
{
     lbfile.read ( reinterpret_cast<char *> ( &target ), sizeof ( target ) );
}
template<>
inline void load_primitive ( fstream &lbfile, string &target )
{
     string result;
     unsigned int size = 0;
     load_primitive<unsigned int> ( lbfile, size );
     for ( int i=0; i<size; ++i ) {
          char c;
          load_primitive<char> ( lbfile, c );
          result += c;
     }
     target = result;
}
template<>
inline void load_primitive ( fstream &lbfile, color_t &c )
{
     unsigned char red;
     unsigned char green;
     unsigned char blue;
     load_primitive<unsigned char> ( lbfile, red );
     load_primitive<unsigned char> ( lbfile, green );
     load_primitive<unsigned char> ( lbfile, blue );
     c = {red, green, blue};
}
template<>
inline void load_primitive ( fstream &lbfile, bool &b )
{
     int n=0;
     load_primitive<int> ( lbfile, n );
     if ( n == 0 ) {
          b = false;
     } else {
          b = true;
     }
}
template<>
inline void load_primitive ( fstream &lbfile, component_type &t )
{
     int n = 0;
     load_primitive<int> ( lbfile, n );
     t = static_cast<component_type> ( n );
}

/* Common Properties */

template<typename T>
inline void save_common_component_properties ( fstream &lbfile, const T &target )
{
     save_primitive<component_type> ( lbfile, target.type );
     save_primitive<int> ( lbfile, target.entity_id );
}

template<typename T>
inline T load_common_component_properties ( fstream &lbfile )
{
     T target;
     load_primitive<int> ( lbfile, target.entity_id );
     return target;
}

/* Specializations for entities */

////// SAVING

template<typename T>
inline void save_component ( fstream &lbfile, const T &target )
{
     save_common_component_properties<T> ( lbfile, target );
     std::cout << "ERROR: Called the generic type of component save.\n";
     throw 101;
}

template<>
inline void save_component ( fstream &lbfile, const calendar_component &target )
{
     save_common_component_properties<calendar_component> ( lbfile, target );
     save_primitive<long> ( lbfile, target.system_time );
     save_primitive<short> ( lbfile, target.year );
     save_primitive<char> ( lbfile, target.month );
     save_primitive<char> ( lbfile, target.day );
     save_primitive<char> ( lbfile, target.hour );
     save_primitive<char> ( lbfile, target.minute );
}

template<>
inline void save_component ( fstream &lbfile, const debug_name_component &target )
{
     save_common_component_properties<debug_name_component> ( lbfile, target );
     save_primitive<string> ( lbfile, target.debug_name );
}

template<>
inline void save_component ( fstream &lbfile, const obstruction_component &target )
{
     save_common_component_properties<obstruction_component> ( lbfile, target );
     save_primitive<bool> ( lbfile, target.blocks_entry );
     save_primitive<bool> ( lbfile, target.blocks_visibility );
}

template<>
inline void save_component ( fstream &lbfile, const position_component &target )
{
     save_common_component_properties<position_component> ( lbfile, target );
     save_primitive<int> ( lbfile, target.x );
     save_primitive<int> ( lbfile, target.y );
}

template<>
inline void save_component ( fstream &lbfile, const power_battery_component &target )
{
     save_common_component_properties<power_battery_component> ( lbfile, target );
     save_primitive<int> ( lbfile, target.storage_capacity );
}

template<>
inline void save_component ( fstream &lbfile, const power_generator_component &target )
{
     save_common_component_properties<power_generator_component> ( lbfile, target );
     save_primitive<int> ( lbfile, target.amount );
     save_primitive<engine::raws::power_generator_condition> ( lbfile, target.generator_mode );
}

template<>
inline void save_component ( fstream &lbfile, const renderable_component &target )
{
     save_common_component_properties<renderable_component> ( lbfile, target );
     save_primitive<unsigned char> ( lbfile, target.glyph );
     save_primitive<color_t> ( lbfile, target.foreground );
     save_primitive<color_t> ( lbfile, target.background );
}

template<>
inline void save_component ( fstream &lbfile, const settler_ai_component &target )
{
     save_common_component_properties<settler_ai_component> ( lbfile, target );
     save_primitive<int> ( lbfile, target.next_tick );
}

template<>
inline void save_component ( fstream &lbfile, const viewshed_component &target )
{
     save_common_component_properties<viewshed_component> ( lbfile, target );
     save_primitive<int> ( lbfile, target.scanner_range );
     save_primitive<viewshed_type> ( lbfile, target.scanner_type );
}

////// LOADING

template<typename T>
inline T load_component ( fstream &lbfile )
{
     T target = load_common_component_properties<T> ( lbfile );
     std::cout << "ERROR: Called the generic type of component load.\n";
     throw 104;
}

template<>
inline calendar_component load_component ( fstream &lbfile )
{
     calendar_component target = load_common_component_properties<calendar_component> ( lbfile );
     load_primitive<long> ( lbfile, target.system_time );
     load_primitive<short> ( lbfile, target.year );
     load_primitive<char> ( lbfile, target.month );
     load_primitive<char> ( lbfile, target.day );
     load_primitive<char> ( lbfile, target.hour );
     load_primitive<char> ( lbfile, target.minute );
     return target;
}

template<>
inline debug_name_component load_component ( fstream &lbfile )
{
     debug_name_component target = load_common_component_properties<debug_name_component> ( lbfile );
     load_primitive<string> ( lbfile, target.debug_name );
     return target;
}

template<>
inline obstruction_component load_component ( fstream &lbfile )
{
     obstruction_component target = load_common_component_properties<obstruction_component> ( lbfile );
     load_primitive<bool> ( lbfile, target.blocks_entry );
     load_primitive<bool> ( lbfile, target.blocks_visibility );
     return target;
}

template<>
inline position_component load_component ( fstream &lbfile )
{
     position_component target = load_common_component_properties<position_component> ( lbfile );
     load_primitive<int> ( lbfile, target.x );
     load_primitive<int> ( lbfile, target.y );
     return target;
}

template<>
inline power_battery_component load_component ( fstream &lbfile )
{
     power_battery_component target = load_common_component_properties<power_battery_component> ( lbfile );
     load_primitive<int> ( lbfile, target.storage_capacity );
     return target;
}

template<>
inline power_generator_component load_component ( fstream &lbfile )
{
     power_generator_component target = load_common_component_properties<power_generator_component> ( lbfile );
     load_primitive<int> ( lbfile, target.amount );
     load_primitive<engine::raws::power_generator_condition> ( lbfile, target.generator_mode );
     return target;
}

template<>
inline renderable_component load_component ( fstream &lbfile )
{
     renderable_component target = load_common_component_properties<renderable_component> ( lbfile );
     load_primitive<unsigned char> ( lbfile, target.glyph );
     load_primitive<color_t> ( lbfile, target.foreground );
     load_primitive<color_t> ( lbfile, target.background );
     return target;
}

template<>
inline settler_ai_component load_component ( fstream &lbfile )
{
     settler_ai_component target = load_common_component_properties<settler_ai_component> ( lbfile );
     load_primitive<int> ( lbfile, target.next_tick );
     return target;
}

template<>
inline viewshed_component load_component ( fstream &lbfile )
{
     viewshed_component target = load_common_component_properties<viewshed_component> ( lbfile );
     load_primitive<int> ( lbfile, target.scanner_range );
     load_primitive<viewshed_type> ( lbfile, target.scanner_type );
     return target;
}

}
}
}