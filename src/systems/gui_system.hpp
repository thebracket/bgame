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

class gui_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
};
