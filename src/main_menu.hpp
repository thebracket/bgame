#pragma once

class main_menu {
public:
	void init();
	void destroy();
	void tick(const double duration_ms);

	int selected = 1;
};

