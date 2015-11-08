#pragma once

#include <iostream>
#include "base_raw.h"
#include "../components/provisions_component.h"
#include "raw_glyph.h"
#include "raw_color_pair.h"

using std::vector;

namespace raws {

struct raw_skill_required : public base_raw {
     raw_skill_required() {
          type = SKILL_REQUIRED;
     };
     
     raw_skill_required(const string skill, const int diff, const int t) : skill_name(skill), difficulty(diff), time (t) {
	  type = SKILL_REQUIRED;
     }
     
     string skill_name;
     int difficulty;
     int time;

     virtual void build_components ( entity &parent, const int &x, const int &y ) const {
     }
};

}
