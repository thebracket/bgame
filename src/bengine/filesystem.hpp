#pragma once

#include <string>

std::string get_save_path();
bool exists(const std::string &filename) noexcept;
