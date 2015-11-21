#pragma once

#include <string>
#include "base_raw.h"

using std::string;

namespace raws
{

struct raw_item_type : public base_raw {
    raw_item_type() {
        type = ITEM_TYPE;
    };
    raw_item_type ( const string t ) : item_type ( t ) {
        type = ITEM_TYPE;
    }
    string item_type;
    
    virtual string get_name_override() override {
	return item_type;
    }

};

}
