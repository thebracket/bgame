#pragma once

#include <rltk.hpp>

class mode_settler_info_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
};
