#pragma once

#include <fstream>
#include "../../engine/virtual_terminal.hpp"

using engine::vterm::color_t;
using std::fstream;

void save_color(const color_t &color, fstream &lbfile);
void load_color(color_t &color, fstream &lbfile);
