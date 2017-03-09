#pragma once

class play_game {
public:
	void init();
	void destroy();
	void tick(const double duration_ms);
private:
	std::function<void()> optional_display_hook = nullptr;
};
