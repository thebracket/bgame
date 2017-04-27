#pragma once

#include <rltk.hpp>

class ai_work_building : public rltk::base_system {
public:
    ai_work_building() { system_name = "AI work - building construction"; }
    virtual void update(const double duration_ms) override final;
    virtual void configure() override final;
private:
};
