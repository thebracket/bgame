#pragma once

#include "base_message.h"
#include <queue>

using std::queue;

namespace engine {

namespace ecs {

class base_system {
public:
  virtual void tick(const double &duration_ms)=0;
};

}

}
