#pragma once

#include <rltk.hpp>

struct tick_message : public rltk::base_message_t {
};

struct slow_tick_message : public rltk::base_message_t {
};

struct hour_elapsed_message : public rltk::base_message_t {
};

struct day_elapsed_message : public rltk::base_message_t {
};

struct action_available_message : public rltk::base_message_t {
    std::size_t entity_id;
    action_available_message() {}
    action_available_message(const std::size_t &id) : entity_id(id) {}
};