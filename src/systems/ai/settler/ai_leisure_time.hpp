#pragma once

#include <rltk.hpp>

class ai_leisure_time_t : public rltk::base_system {
public:
    ai_leisure_time_t() { system_name = "AI Leisure Time"; }
    virtual void update(const double duration_ms) override final;
    virtual void configure() override final;
private:
};
