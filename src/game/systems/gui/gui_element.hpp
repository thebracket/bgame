#pragma once

#include <string>

struct gui_element {
public:
	std::string parent;

	virtual void render()=0;
	int x=0;
	int y=0;
};
