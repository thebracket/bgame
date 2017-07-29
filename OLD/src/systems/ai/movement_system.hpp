#pragma once

#include <rltk.hpp>
#include "../../components/position.hpp"
#include "../../utils/octree.hpp"

extern octree_t entity_octree;

class movement_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
};
