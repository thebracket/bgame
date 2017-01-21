#pragma once

#include <rltk.hpp>
#include "boost/container/flat_map.hpp"

class trigger_system : public rltk::base_system {
public:
    virtual void update(const double duration_ms) override final;
    virtual void configure() override final;
private:
    bool dirty = true;
    boost::container::flat_map<int, std::size_t> triggers;
    std::size_t trigger_id = 0;
};
