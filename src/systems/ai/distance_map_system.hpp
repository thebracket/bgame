#pragma once

#include <rltk.hpp>
#include "../../utils/dijkstra_map.hpp"
#include "../../components/position.hpp"

extern dijkstra_map huntables_map;
extern dijkstra_map butcherables_map;
extern dijkstra_map bed_map;
extern dijkstra_map settler_map;
extern dijkstra_map architecure_map;
extern dijkstra_map blocks_map;
extern dijkstra_map levers_map;
extern bool dijkstra_debug;
extern dijkstra_map axe_map;
extern dijkstra_map pick_map;

class distance_map_system : public rltk::base_system {
public:
    virtual void update(const double duration_ms) override final;
    virtual void configure() override final;
private:
    bool update_huntables = true;
    bool update_butcherables = true;
    bool update_bed_map = true;
    bool update_settler_map = true;
    bool update_architecture_map = true;
    bool update_blocks_map = true;
    bool update_levers_map = true;
    bool update_axe_map = true;
    bool update_pick_map = true;
};
