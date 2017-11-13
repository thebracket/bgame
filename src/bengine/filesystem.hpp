#pragma once

#include "../stdafx.h"

std::string get_save_path();
bool exists(const std::string &filename) noexcept;
void remove_from_path(std::string &s, const std::string needle);