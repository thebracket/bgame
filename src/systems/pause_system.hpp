#pragma once

#include <rltk.hpp>

class pause_system : public rltk::base_system {
public:
    virtual void configure() override final;
    virtual void update(const double duration_ms) override final;
private:
    bool requested_pause = false;
    bool requested_step = false;
};
