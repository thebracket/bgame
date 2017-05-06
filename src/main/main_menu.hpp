#pragma once

#include <string>

class main_menu {
public:
	void init();
	void destroy();
	void tick(const double duration_ms);

	int selected = 1;
	double key_delay = 0.0;
	bool clicked = false;
	bool world_exists = false;

private:
	std::string tagline;
	std::string get_descriptive_noun() const;
	char online_username[254];
	bool show_options = false;
};

