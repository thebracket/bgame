#pragma once

#include <rltk.hpp>

class ai_status_effects : public rltk::base_system {
public:
    ai_status_effects() { system_name = "AI Status Effects"; }
    virtual void update(const double duration_ms) override final;
    virtual void configure() override final;
private:
};
