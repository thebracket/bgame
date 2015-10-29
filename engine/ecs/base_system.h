#pragma once

namespace engine {

namespace ecs {

class base_system {
public:
  virtual void tick(const double &duration_ms)=0;
};

}

}
