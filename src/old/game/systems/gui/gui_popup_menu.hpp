#pragma once

#include "gui_element.hpp"
#include "../../game.h"
#include <string>
#include <vector>
#include <functional>

struct gui_menu_option {
	std::string title;
	std::function<void()> action;
};

class gui_popup_menu : public gui_element {
public:
	gui_popup_menu(const std::string &n_title, const int &sx, const int &sy) : title(n_title), screen_x(sx), screen_y(sy) {}
	void add_option(const gui_menu_option &option);
	virtual void render() override;
private:
	std::string title;
	std::vector<gui_menu_option> options;
	int screen_x;
	int screen_y;
};
