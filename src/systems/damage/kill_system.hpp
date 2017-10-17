#pragma once

#include <string>

namespace systems {
	namespace kill_system {

		struct entity_slain_message  {
			entity_slain_message() {}
			entity_slain_message(const std::size_t id) : victim(id) {}
			entity_slain_message(const std::size_t id, const std::string &cause) : victim(id), cause_of_death(cause) {}
			std::size_t victim;
			std::string cause_of_death = "";
		};

		void run(const double &duration_ms);
		void fatality(entity_slain_message msg);
	}
}