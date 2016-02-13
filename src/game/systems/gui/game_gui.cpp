#include "game_gui.hpp"
#include <algorithm>

void game_gui::add_element(const std::string &name, std::unique_ptr<gui_element> &&element) {
	elements.push_back(std::make_pair(name,std::move(element)));
}

void game_gui::delete_element(const std::string &name) {
	std::vector<std::string> child_list;
	std::for_each(elements.begin(), elements.end(), [&child_list, &name] (std::pair<std::string, std::unique_ptr<gui_element>> &e) {
		if (e.second->parent == name) child_list.push_back(e.second->parent);
	});

	for (const std::string &child : child_list) {
		delete_element(child);
	}

	auto finder = std::find_if(elements.begin(), elements.end(), [&name] (std::pair<std::string, std::unique_ptr<gui_element>> &e) { return (e.first == name); });
	if (finder != elements.end()) {
		elements.erase(std::remove_if(elements.begin(), elements.end(), [&name] (std::pair<std::string, std::unique_ptr<gui_element>> &e) { return (e.first == name); }));
	}
}

void game_gui::delete_all() {
	elements.clear();
}

gui_element * game_gui::get_element(const std::string &name) {
	auto finder = std::find_if(elements.begin(), elements.end(), [&name] (std::pair<std::string, std::unique_ptr<gui_element>> &e) { return (e.first == name); });
	if (finder == elements.end()) return nullptr;
	return finder->second.get();
}

void game_gui::render() {
	std::vector<std::string> closing;
	std::for_each(elements.begin(), elements.end(), [&closing] (std::pair<std::string, std::unique_ptr<gui_element>> &e) {
		e.second->render();
		if (e.second->request_close) {
			closing.push_back(e.first);
		}
	});

	while (!closing.empty()) {
		const std::string target = closing[0];
		closing.erase(closing.begin());
		delete_element(target);
	}
}
