#include "designations.hpp"

void designations_t::to_xml(xml_node * c) {
    component_to_xml(c,
        std::make_pair("mining", mining),
        std::make_pair("architecture", architecture),
        std::make_pair("chopping", chopping),
        std::make_pair("buildings", buildings),
        std::make_pair("build_orders", build_orders),
        std::make_pair("guard_points", guard_points),
        std::make_pair("deconstructions", deconstructions),
        std::make_pair("harvest", harvest),
        std::make_pair("current_power", current_power),
        std::make_pair("current_cash", current_cash),
        std::make_pair("standing_order_idle_move", standing_order_idle_move),
        std::make_pair("standing_order_wildlife_treatment", standing_order_wildlife_treatment),
        std::make_pair("standing_order_upgrade", standing_order_upgrade)
    );
}

void designations_t::from_xml(xml_node * c) {
    c->iterate_child("mining", [this] (xml_node *ch) {
        mining[ ch->val<int>("key") ] = ch->val<uint8_t>("v");
    });
    c->iterate_child("architecture", [this] (xml_node *ch) {
        architecture[ ch->val<int>("key") ] = ch->val<uint8_t>("v");
    });
    c->iterate_child("chopping", [this] (xml_node *ch) {
        position_t pos;
        pos.from_xml(ch);
        chopping [ ch->val<int>("key")] = pos;
    });
    c->iterate_child("buildings", [this] (xml_node *ch) {
        building_designation_t b;
        b.from_xml(ch);
        buildings.push_back(b);
    });
    c->iterate_child("build_orders", [this] (xml_node *ch) {
        std::pair<uint8_t, std::string> bo{
            ch->find("build_orders_first")->val<uint8_t>("build_orders"),
            ch->find("build_orders_second")->val<std::string>("build_orders")
        };
    });
    c->iterate_child("guard_points", [this] (xml_node *ch) {
        position_t pos;
        pos.from_xml(ch->find("guard_points_second"));
        std::pair<bool, position_t> gp{
            ch->find("guard_points_first")->val<bool>("guard_points"),
            pos
        };
        guard_points.push_back(gp);
    });
    c->iterate_child("harvest", [this] (xml_node *ch) {
        position_t pos;
        pos.from_xml(ch->find("harvest_second"));
        std::pair<bool, position_t> gp{
                ch->find("harvest_first")->val<bool>("harvest"),
                pos
        };
        harvest.push_back(gp);
    });
    c->iterate_child("deconstructions", [this] (xml_node *ch) {
        unbuild_t u;
        u.from_xml(ch);
        deconstructions.push_back(u);
    });
    current_power = c->val<int>("current_power");
    current_cash = c->val<uint64_t>("current_cash");
    standing_order_idle_move = c->val<uint8_t>("standing_order_idle_move");
    standing_order_wildlife_treatment = c->val<uint8_t>("standing_order_wildlife_treatment");
    standing_order_upgrade = c->val<uint8_t>("standing_order_upgrade");
}