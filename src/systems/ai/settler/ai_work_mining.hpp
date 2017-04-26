#pragma once

#include <rltk.hpp>

class ai_work_mining : public rltk::base_system {
public:
    ai_work_mining() { system_name = "AI work - mining"; }
    virtual void update(const double duration_ms) override final;
    virtual void configure() override final;
private:
};
