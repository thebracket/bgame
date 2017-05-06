#pragma once

#include <rltk.hpp>

class ai_new_arrival : public rltk::base_system {
public:
    ai_new_arrival() { system_name = "AI New Arrivals"; }
    virtual void update(const double duration_ms) override final;
    virtual void configure() override final;
private:
};
