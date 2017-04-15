#pragma once

#include <rltk.hpp>

class ai_work_time : public rltk::base_system {
public:
    ai_work_time() { system_name = "AI work mode"; }
    virtual void update(const double duration_ms) override final;
    virtual void configure() override final;
private:
};
