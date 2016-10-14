#pragma once

#include <rltk.hpp>
#include <boost/container/flat_map.hpp>
#include <vector>

extern boost::container::flat_map<int, std::vector<rltk::vchar>> renderables;

class renderables_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
	bool renderables_changed = true;
};