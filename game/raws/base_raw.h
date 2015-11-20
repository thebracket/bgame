#pragma once

#include <vector>
#include <memory>
#include <utility>
#include <string>
#include "../../engine/ecs.hpp"

using std::vector;
using std::unique_ptr;
using engine::entity;
using std::string;
using std::pair;

namespace raws {

enum tag_type { RENDERABLE, NAME, DESCRIPTION, GLYPH, COLOR_PAIR, OBSTRUCTION, POWER_GENERATOR, POWER_BATTERY, SETTLER_ACTION, BUILDABLE, SKILL_REQUIRED, COMPONENT, SLEEPABLE, TILE, LAYER };

struct base_raw {
     tag_type type;
     vector<unique_ptr<base_raw>> children;

     virtual void build_components ( entity &parent, const int &x, const int &y, const bool placeholder=false ) const {
          for ( const unique_ptr<base_raw> &child : children ) {
               child->build_components ( parent, x, y, placeholder );
          }
     }
     
     
     bool get_buildable( string &name ) {
       bool is_buildable = false;
       
       for ( const unique_ptr<base_raw> &child : children ) {
       
	  if ( child->type == BUILDABLE ) is_buildable = true;
	  if ( child->type == NAME ) name = child->get_name_override();
       }
       
       return is_buildable;
     }
     
     vector<string> get_buildable_requirements() {
          vector<string> result;
	  get_buildable_requirements_recurse( result );
          return result;
     }

     virtual void get_buildable_requirements_recurse ( vector<string> &result ) {
          for ( const unique_ptr<base_raw> &child : children ) {
               child->get_buildable_requirements_recurse ( result );
          }
     }
  
     virtual string get_name_override() {
       return "";
     }
  
};

}
