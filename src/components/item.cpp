#include "item.hpp"

void item_t::to_xml(xml_node * c) {
    component_to_xml(c,
        std::make_pair("item_name", item_name),
        std::make_pair("item_tag", item_tag),
        std::make_pair("category", category),
        std::make_pair("type", type),
        std::make_pair("material", material),
        std::make_pair("claimed", claimed),
        std::make_pair("stack_size", stack_size),
        std::make_pair("clothing_glyph", clothing_glyph),
        std::make_pair("clothing_color", clothing_color)
    );
}

void item_t::from_xml(xml_node * c) {
    item_name = c->val<std::string>("item_name");
    item_tag = c->val<std::string>("item_tag");
    std::string cats = c->val<std::string>("category");
    category = std::bitset<NUMBER_OF_ITEM_CATEGORIES>(cats);
    type = (item_type_t)c->val<int>("type");
    material = c->val<std::size_t>("material");
    claimed = c->val<bool>("claimed");
    stack_size = c->val<int>("stack_size");
    clothing_glyph = c->val<uint16_t>("clothing_glyph");
    clothing_color = c->color("clothing_color");
}