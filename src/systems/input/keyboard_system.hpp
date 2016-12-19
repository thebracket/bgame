#pragma once

#include <rltk.hpp>

class keyboard_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
	void process_input(sf::Event &e);
};

extern bool collect_text;
extern std::string input_text;
