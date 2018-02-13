#pragma once

namespace first_run_screen {
	void tick(const double &duration_ms) noexcept;
	bool is_first_run() noexcept;
	extern bool first_run_done;
}
