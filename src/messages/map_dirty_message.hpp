#pragma once

#include <rltk.hpp>

struct map_dirty_message : public rltk::base_message_t {
};


struct map_rerender_message : public rltk::base_message_t {
};
