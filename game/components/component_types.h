#pragma once

#include <fstream>
#include <string>
#include "../../engine/virtual_terminal.h"

using std::fstream;
using std::string;
using engine::vterm::color_t;

enum component_type{dummy,position,name,renderable,viewshed,calendar,settler_ai,
  obstruction,power_generator,power_battery,gamestats,gamespecies,gamehealth,provision,
  item_storage, item_carried
};

namespace serialization_generic {
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
     for ( std::size_t i=0; i<size; ++i ) {
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
     c = color_t{red, green, blue};
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
inline void load_common_component_properties ( fstream &lbfile, T &target )
{
     load_primitive<int> ( lbfile, target.entity_id );
}

}
