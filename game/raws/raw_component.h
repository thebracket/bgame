#pragma once

#include <iostream>
#include "base_raw.h"
#include "../components/provisions_component.h"
#include "raw_glyph.h"
#include "raw_color_pair.h"

using std::vector;

namespace raws {

struct raw_component : public base_raw {
     raw_component() {
          type = COMPONENT;
     };
     
     raw_component(const string &item, const int &qty) : required_item(item), quantity(qty) {
	  type = COMPONENT;
     }
     
     string required_item;
     int quantity;
     
     virtual void get_buildable_requirements_recurse ( vector<string> &result ) override {
	  for (int i=0; i<quantity; ++i) {
	      result.push_back( required_item );
	  }
       
          for ( const unique_ptr<base_raw> &child : children ) {
               child->get_buildable_requirements_recurse ( result );
          }
     }
};

}
