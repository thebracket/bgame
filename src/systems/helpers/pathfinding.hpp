#pragma once

#include "../../components/position.hpp"
#include <memory>
#include <deque>

struct navigation_path_t
{
	bool success = false;
	std::deque<position_t> steps{};
	position_t destination{0,0,0};
};

std::shared_ptr<navigation_path_t> find_path(const position_t &start, const position_t &end, const bool find_adjacent = false, const std::size_t civ = 0) noexcept;
