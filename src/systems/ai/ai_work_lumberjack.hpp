#pragma once

#include <rltk.hpp>

class ai_work_lumberjack : public rltk::base_system {
public:
    ai_work_lumberjack() { system_name = "AI work - lumberjack"; }
    virtual void update(const double duration_ms) override final;
    virtual void configure() override final;
private:
};
