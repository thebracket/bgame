#pragma once

#include <rltk.hpp>
#include <unordered_set>

class visibility_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
	std::unordered_set<std::size_t> dirty_entities;
	bool dirty = true;
};

