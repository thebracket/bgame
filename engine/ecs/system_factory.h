#pragma once

#include "test_system.h"
#include <memory>

using std::make_unique;
using std::unique_ptr;

namespace engine {
namespace ecs {

inline unique_ptr<test_system> make_test_system() { return make_unique<test_system>(); };
  
}
}
