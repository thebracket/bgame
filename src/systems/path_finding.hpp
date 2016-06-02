#pragma once

#include "../components/position.hpp"
#include <rltk.hpp>
#include <memory>

std::shared_ptr<rltk::navigation_path<position_t>> find_path(const position_t &start, const position_t &end);
