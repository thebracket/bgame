#pragma once

#include <rltk.hpp>

class ai_work_hunt  : public rltk::base_system {
public:
    ai_work_hunt() { system_name = "AI work - hunting"; }
    virtual void update(const double duration_ms) override final;
    virtual void configure() override final;
private:
};
