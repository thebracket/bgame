#pragma once

#include <rltk.hpp>

struct map_dirty_message : public rltk::base_message_t {
};

struct architecture_changed_message : public rltk::base_message_t {
};

struct blocks_changed_message : public rltk::base_message_t {
};

struct triggers_changes_message : public rltk::base_message_t {
};


struct map_changed_message : public rltk::base_message_t {
};

struct map_rerender_message : public rltk::base_message_t {
};

struct opacity_changed_message : public rltk::base_message_t {
};

struct leverpull_changed_message : public rltk::base_message_t {
};

struct axemap_changed_message : public rltk::base_message_t {
};


struct tile_removed_message : public rltk::base_message_t {
    tile_removed_message() {}
    tile_removed_message(const int &X, const int &Y, const int &Z) : x(X), y(Y), z(Z) {}
    int x, y, z;
};