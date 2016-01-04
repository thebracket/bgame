#pragma once

#include <vector>
#include <memory>
#include <string>
#include "gui_element_base.h"

using std::vector;
using std::unique_ptr;
using std::string;

namespace engine
{

class gui
{
public:
	gui();

	std::pair<int, int> get_screen_size()
	{
		return std::make_pair(screen_width, screen_height);
	}
	;

	void add_child(unique_ptr<gui_element_base> child)
	{
		children.push_back(std::move(child));
	}
	;
	gui_element_base * find_child(string tag);

	void render();
private:
	int screen_width;
	int screen_height;
	vector<unique_ptr<gui_element_base>> children;
};

}
