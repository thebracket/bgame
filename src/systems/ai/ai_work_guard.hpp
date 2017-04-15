#pragma once

#include <rltk.hpp>

class ai_work_guard : public rltk::base_system {
public:
    ai_work_guard() { system_name = "AI work - guard"; }
    virtual void update(const double duration_ms) override final;
    virtual void configure() override final;
private:
};
