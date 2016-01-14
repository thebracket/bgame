#pragma once

#include "gui_element.hpp"
#include "../../game.h"

class gui_static_text : public gui_element {
public:
	gui_static_text(const std::string &fnt, const std::string &msg, const SDL_Color &col, const int &X, const int &Y) : font(fnt), message(msg), color(col) {
		x = X;
		y = Y;
	}
	virtual void render() override;

	std::string font;
	std::string message;
	SDL_Color color;
};
