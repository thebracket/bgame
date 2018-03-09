#pragma once

namespace systems {
	namespace fluids {
		void copy_to_shader();
		void copy_back_to_world();
		void run(const double &duration_ms);
	}
}