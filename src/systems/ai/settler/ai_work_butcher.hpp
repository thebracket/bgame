#pragma once

#include <rltk.hpp>

class ai_work_butcher  : public rltk::base_system {
public:
    ai_work_butcher() { system_name = "AI work - butcher"; }
    virtual void update(const double duration_ms) override final;
    virtual void configure() override final;
private:
};
