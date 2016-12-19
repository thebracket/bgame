#pragma once

#include <rltk.hpp>

class kill_system : public rltk::base_system {
public:
    virtual void update(const double duration_ms) override final;
    virtual void configure() override final;
private:
    void inflict_death();
};

