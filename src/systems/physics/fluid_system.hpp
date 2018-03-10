#pragma once

namespace systems {
	namespace fluids {
		void run(const double &duration_ms);
		void copy_to_gpu();
		void copy_from_gpu();

		extern bool water_dirty;
	}
}