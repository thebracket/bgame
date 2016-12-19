#pragma once

#include <rltk.hpp>

class sentient_ai_system : public rltk::base_system {
public:
	sentient_ai_system() { system_name = "Sentient AI"; }
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
};
