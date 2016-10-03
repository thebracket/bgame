#pragma once

#include <rltk.hpp>
#include <memory>
#include <string>
#include "gui_elements.hpp"

extern std::vector<std::unique_ptr<base_gui>> gui_elements;

template <class C, typename ... Ts>
inline void add_gui_element(Ts... args) {
    gui_elements.emplace_back(std::make_unique<C>(args...));
}

template <class C>
inline void add_gui_element(std::unique_ptr<C> &&element) {
	gui_elements.push_back(std::move(element));
}

struct dialog_placement_t {
	dialog_placement_t() : w(rltk::term(3)->term_width), h(rltk::term(3)->term_height),
			left(3), right(rltk::term(3)->term_width-3), top(2), bottom(rltk::term(3)->term_height-2)
	{		
	}

	const int w;
	const int h;

    const int left;
    const int right;
    const int top;
    const int bottom;
};

class gui_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
};
