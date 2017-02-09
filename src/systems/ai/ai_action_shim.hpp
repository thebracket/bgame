#pragma once

#include <rltk.hpp>

class ai_action_shim : public rltk::base_system {
public:
    ai_action_shim() { system_name = "AI Shim - convert to old timing"; }
    virtual void update(const double duration_ms) override final;
    virtual void configure() override final;
private:
};
