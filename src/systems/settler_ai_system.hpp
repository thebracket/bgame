#pragma once

#include <rltk.hpp>

class settler_ai_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override;
	virtual void configure() override;
};
