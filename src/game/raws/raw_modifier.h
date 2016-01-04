#pragma once

#include <string>
#include <iostream>
#include "base_raw.h"
#include "../../engine/virtual_terminal.hpp"

using std::string;

namespace raws {

struct raw_modifier : public base_raw {
	 virtual ~raw_modifier() {}

     raw_modifier() {
          type = MODIFIER;
     };
     raw_modifier ( const string &stat, const int &mod ) : stat_to_modify ( stat ), modifier ( mod ) {
          type=MODIFIER;
     }
     string stat_to_modify;
     int modifier;

     virtual int get_modifier ( const string &stat ) override {
          if ( stat == stat_to_modify ) {
	       //std::cout << "Modifier to " << stat << " = " << modifier << "\n";
               return modifier;
          } else {
	       //std::cout << "Modifier to " << stat << " = 0\n";
               return 0;
          }
     }
};

}
