#pragma once

#include <rltk.hpp>
#include "../components/components.hpp"
#include "../raws/raws.hpp"

struct inventory_item_t {
	std::size_t id;
	bool claimed = false;
	boost::optional<position_t> pos;
	std::bitset<NUMBER_OF_ITEM_CATEGORIES> categories;
	std::string item_name;
	std::string item_tag;
};

class inventory_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
	bool dirty = true;
};

bool is_item_category_available(const int &category);