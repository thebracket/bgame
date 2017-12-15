#pragma once

namespace first_run_screen {
	void tick(const double &duration_ms);
	bool is_first_run();
	extern bool first_run_done;
}
