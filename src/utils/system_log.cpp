#include "stdafx.h"
#include "system_log.hpp"
#include <mutex>
#include <iostream>

namespace system_log
{
	constexpr int N_LOGS = 2;

	struct log_entry_t
	{
		log_severity severity;
		std::string message;
	};

	static std::array<std::vector<log_entry_t>, N_LOGS> logs;
	std::mutex write_lock;
}

void gamelog(const log_target target, const log_severity severity, const std::string& entry) noexcept
{
	std::lock_guard<std::mutex> lock(system_log::write_lock);
	system_log::logs[static_cast<int>(target)].emplace_back(system_log::log_entry_t{severity, entry});
	std::cout << entry << "\n";
}

std::vector<std::string> ten_most_recent_log_entries(const int n) noexcept
{
	std::vector<std::string> result;
	std::lock_guard<std::mutex> lock(system_log::write_lock);
	if (system_log::logs[n].empty()) return result;

	auto start = static_cast<int>(system_log::logs.size()) - 10;
	if (start < 0) start = 0;
	for (auto i = start; i < system_log::logs.size(); ++i)
	{
		result.emplace_back(system_log::logs[n][i].message);
	}
	return result;
}
