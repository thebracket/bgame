#include "path_finding.h"
#include "../world/world.h"
#include <unordered_set>
#include <tuple>
#include <map>
#include "../components/position_component3d.hpp"

using std::unordered_set;
using std::tuple;
using std::make_tuple;
using std::fill;
using std::map;
using std::get;
using std::swap;
using std::make_shared;
using std::make_pair;

namespace ai {

shared_ptr< navigation_path > find_path ( location_t start, location_t end )
{
    shared_ptr<navigation_path> result;
    result->destination = end;
    result->steps.push( end );
    return result;
}
  
}
