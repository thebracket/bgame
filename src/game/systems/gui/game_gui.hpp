#pragma once

#include "gui_element.hpp"

#include <memory>
#include <vector>
#include <utility>
#include <string>

class game_gui {
public:
	void add_element(const std::string &name, std::unique_ptr<gui_element> &&element);
	void delete_element(const std::string &name);
	void delete_all();
	gui_element * get_element(const std::string &name);

	void render();
private:
	std::vector<std::pair<std::string, std::unique_ptr<gui_element>>> elements;

};
