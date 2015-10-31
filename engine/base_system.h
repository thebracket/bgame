#pragma once

namespace engine {

class base_system {
public:
  virtual void tick(const double &duration_ms)=0;
};

}
