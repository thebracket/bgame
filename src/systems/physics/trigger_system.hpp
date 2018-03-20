#pragma once

#include "movement_system.hpp"
#include "../../utils/thread_safe_message_queue.hpp"

namespace systems {
	namespace triggers {
		void run(const double &duration_ms);
		void entry_trigger_firing(const systems::movement::entity_moved_message &msg);
		void edit_triggers();

		struct triggers_changed_message {
		};

		struct request_lever_pull_message {
			request_lever_pull_message() = default;
			request_lever_pull_message(const int &lid) : lever_id(lid) {}
			int lever_id;
		};

		struct leverpull_changed_message {
		};

		struct trigger_details_requested {
			trigger_details_requested() = default;
			trigger_details_requested(const int &lid) : lever_id(lid) {}
			int lever_id;
		};

		struct lever_pulled_message {
			lever_pulled_message() = default;
			lever_pulled_message(const int &lid) : lever_id(lid) {}
			int lever_id;
		};

		extern thread_safe_message_queue<triggers_changed_message> triggers_changed;
		extern thread_safe_message_queue<request_lever_pull_message> lever_pull_requests;
		extern thread_safe_message_queue<leverpull_changed_message> lever_pull_changes;
		extern thread_safe_message_queue<trigger_details_requested> trigger_details;
		extern thread_safe_message_queue<lever_pulled_message> lever_pulled;
	}
}
