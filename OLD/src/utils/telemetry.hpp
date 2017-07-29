#pragma once

#include <string>

struct telemetry_t {
    std::string event;
    std::string details;
};

void start_telemetry();
void stop_telemetry();
void call_home(const std::string &event, const std::string details="");
