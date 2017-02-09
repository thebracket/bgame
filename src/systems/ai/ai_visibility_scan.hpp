#pragma once

#include <rltk.hpp>

class ai_visibility_scan : public rltk::base_system {
public:
    ai_visibility_scan() { system_name = "AI Visibility Scanner"; }
    virtual void update(const double duration_ms) override final;
    virtual void configure() override final;
private:
};
