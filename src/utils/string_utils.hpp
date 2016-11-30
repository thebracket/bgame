#pragma once

#include <vector>
#include <string>

std::vector<std::string> split ( const std::string &str, const char &delimiter );
std::string to_proper_noun_case(const std::string &original);
std::string max_width_str(const std::string &original, const int &width);
std::string str_replace(std::string &s, const std::string &toReplace, const std::string &replaceWith);
bool str_contains(const std::string &s, const std::string &search_for);
