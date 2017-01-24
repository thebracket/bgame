#pragma once

#include <rltk.hpp>
#include <memory>
#include "../../components/settler_ai.hpp"
#include "../../components/designations.hpp"

class workflow_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
    bool dirty = true;
};

std::unique_ptr<reaction_task_t> find_automatic_reaction_task(const settler_ai_t &ai);
std::unique_ptr<reaction_task_t> find_queued_reaction_task(const settler_ai_t &ai);
void free_workshop(const std::size_t &id);
