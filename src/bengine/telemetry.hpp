#pragma once

#include "../stdafx.h"

namespace telemetry {
	void on_tick();
}

void start_telemetry();
void stop_telemetry();
void call_home(const std::string &category, const std::string &action);
void call_home(const std::string &category, const std::string &action, const std::string &label);
void call_home(const std::string &category, const std::string &action, const std::string &label, const uint32_t &value);
