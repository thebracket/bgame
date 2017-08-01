#pragma once

void start_telemetry();
void stop_telemetry();
void call_home(const std::string &event, const std::string details="");
