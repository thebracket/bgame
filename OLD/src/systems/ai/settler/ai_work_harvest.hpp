#pragma once

#include <rltk.hpp>

class ai_work_harvest : public rltk::base_system {
public:
    ai_work_harvest() { system_name = "AI work - harvest"; }
    virtual void update(const double duration_ms) override final;
    virtual void configure() override final;
private:
};
