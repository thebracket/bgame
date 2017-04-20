#pragma once

#include <rltk.hpp>
#include "../../components/ai_tag_work_lumberjack.hpp"
#include "ai_work_template.hpp"

class ai_work_lumberjack : public ai_work_template<ai_tag_work_lumberjack> {
public:
    ai_work_lumberjack() { system_name = "AI work - lumberjack"; }
    virtual void update(const double duration_ms) override final;
    virtual void configure() override final;
private:
};
