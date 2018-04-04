#pragma once

#include <string>
#include <fmt/format.h>

enum class log_severity { info, warning, error };
enum class log_target { LOADER=0, GAME=1 };

void gamelog(const log_target target, const log_severity severity, const std::string &entry) noexcept;

template<typename ...Cs>
void glog(const log_target target, const log_severity severity, Cs... fmt) noexcept
{
	gamelog(target, severity, fmt::format(fmt...));
}

std::vector<std::string> ten_most_recent_log_entries(const int n) noexcept;