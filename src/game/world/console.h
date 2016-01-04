#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include "../../engine/virtual_terminal.hpp"

using std::string;
using std::vector;
using engine::vterm::color_t;

struct console_character
{
	unsigned char character;
	color_t color;
};

struct log_entry
{
	vector<console_character> line;
};

class console
{
public:
	void write(const string line);
	vector<log_entry>::iterator last_n(std::size_t n)
	{
		return log.end() - std::min(log.size(), n);
	}
	;
	vector<log_entry>::iterator end()
	{
		return log.end();
	}
	;

private:
	vector<log_entry> log;
};
