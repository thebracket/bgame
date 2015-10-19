#pragma once

#include "camera_system.h"
#include "renderable_system.h"
#include "viewshed_system.h"
#include "calendar_system.h"
#include "settler_ai_system.h"
#include <memory>

using std::make_unique;
using std::unique_ptr;

namespace engine {
namespace ecs {

inline unique_ptr<camera_system> make_camera_system() { return make_unique<camera_system>(); };
inline unique_ptr<renderable_system> make_renderable_system() { return make_unique<renderable_system>(); };
inline unique_ptr<viewshed_system> make_viewshed_system() { return make_unique<viewshed_system>(); };
inline unique_ptr<calendar_system> make_calendar_system() { return make_unique<calendar_system>(); };
inline unique_ptr<settler_ai_system> make_settler_ai_system() { return make_unique<settler_ai_system>(); };
  

}
}
