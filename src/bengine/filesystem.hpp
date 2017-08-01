#pragma once

#include <string>

std::string get_save_path();
bool exists(const std::string &filename) noexcept;
void remove_from_path(std::string &s, const std::string needle);