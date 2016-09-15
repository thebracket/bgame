#pragma once

#include <rltk.hpp>

struct tick_message : public rltk::base_message_t {
};

struct hour_elapsed_message : public rltk::base_message_t {
};

struct day_elapsed_message : public rltk::base_message_t {
};
