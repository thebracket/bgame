#pragma once

#include <mutex>
#include <queue>

template <typename MESSAGE_TYPE>
class thread_safe_message_queue {
public:
	void enqueue(MESSAGE_TYPE &&msg) {
		std::lock_guard<std::mutex> lock(queue_mutex);
		queue.emplace(msg);
	}

	template <typename F>
	void process_one(F && func) {
		std::lock_guard<std::mutex> lock(queue_mutex);
		if (!queue.empty()) {
			MESSAGE_TYPE msg = queue.front();
			queue.pop();
			func(msg);
		}
	}

	template <typename F>
	void process_all(F && func) {
		std::lock_guard<std::mutex> lock(queue_mutex);
		while (!queue.empty()) {
			MESSAGE_TYPE msg = queue.front();
			queue.pop();
			func(msg);
		}
	}

private:
	std::mutex queue_mutex;
	std::queue<MESSAGE_TYPE> queue;
};