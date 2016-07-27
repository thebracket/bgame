#pragma once

#include <rltk.hpp>
#include "../messages/perform_mining.hpp"
#include "../messages/perform_construction.hpp"

class topology_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
    void dig(const perform_mining_message &e);
    void channel(const perform_mining_message &e);
    void ramp(const perform_mining_message &e);
    void stairs_up(const perform_mining_message &e);
    void stairs_down(const perform_mining_message &e);
    void stairs_updown(const perform_mining_message &e);
    void recalculate(const perform_mining_message &e);
    void spawn_mining_result(const perform_mining_message &e);
    void spawn_mining_result_impl(const perform_mining_message &e, std::string tag);

    void build_construction(const perform_construction_message &e);
};
