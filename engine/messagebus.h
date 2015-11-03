#pragma once

#include <tuple>
#include <vector>
#include <algorithm>
#include "mpl_foreach.h"

using std::tuple;
using std::vector;

using engine::ecs_detail::for_each;

namespace engine {

template<typename... messages_list>
class message_bus {
private:
     tuple<vector<messages_list>...> storage;

     template <typename T>
     vector<T> * find_appropriate_bag() {
          return &std::get< vector<T> > ( storage );
     }

public:
     message_bus() {
     }

     void clear() {
          for_each ( storage, [] ( auto &x ) {
	       if (!x.empty()) {
		  std::for_each(x.begin(), x.end(), [] (auto &n) { --n.ttl; if (n.ttl <= 0) n.deleted = true; });
		  auto new_end = std::remove_if(x.begin(), x.end(), [] (auto &n) { return n.deleted; });
		  x.erase(new_end, x.end());
	       }
          } );
     }

     template<typename T>
     void add_message ( T new_message ) {
          vector<T> * bag = find_appropriate_bag<T>();
          bag->push_back ( new_message );
     }

     template<typename T>
     vector<T> * get_messages_by_type() {
          return find_appropriate_bag<T>();
     }
};

}