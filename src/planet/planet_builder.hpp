#pragma once

#include <rltk.hpp>
#include <mutex>
#include <vector>
#include <string>
#include <memory>
#include "region.hpp"

void build_planet();
bool is_planet_build_complete();
extern std::mutex planet_builder_lock;
extern std::unique_ptr<std::vector<rltk::vchar>> planet_builder_display;
extern std::string planet_builder_status;

void setup_build_planet(int width, int height);
