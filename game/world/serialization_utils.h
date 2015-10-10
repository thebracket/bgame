#pragma once

#include "../../engine/virtual_terminal.h"
#include <fstream>

using engine::vterm::color_t;
using std::fstream;

void save_color(const color_t &color, fstream &lbfile);
