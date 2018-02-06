#pragma once

#include <functional>
#include <string>

struct life_event_template; // Forward

life_event_template * get_life_event(const std::string tag);
void each_life_event(const std::function<void(std::string tag, life_event_template *)> func);

/*
 * Read Lua definitions.
 */
void read_life_events() noexcept;

