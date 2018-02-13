#pragma once

#include <functional>

struct building_def_t; // Forward declaration
struct graphviz_t; // Forward

building_def_t * get_building_def(const std::string &tag) noexcept;
void each_building_def(const std::function<void(building_def_t *)> &func) noexcept;

void read_buildings() noexcept;
void sanity_check_buildings() noexcept;
void make_building_tree(graphviz_t * tree);
