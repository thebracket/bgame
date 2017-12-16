#pragma once

#include <experimental/coroutine>

struct generator {
	struct promise_type {
		int current_value = 0;

		promise_type() {
		}
		~promise_type() {
		}
		auto initial_suspend() {
			return std::experimental::suspend_always{};
		}
		auto final_suspend() {
			return std::experimental::suspend_always{};
		}
		auto get_return_object() {
			return generator{ handle_type::from_promise(*this) };
		}
		void unhandled_exception() {
			std::cout << "Exception...." << std::endl;
			std::exit(1);
		}
		auto return_void() {
			return std::experimental::suspend_never{};
		}
		auto yield_value(int value) {
			current_value = value;
			return std::experimental::suspend_always{};
		}
	};
	using handle_type =
		std::experimental::coroutine_handle<promise_type>;

	bool move_next() {
		coro.resume();
		//std::cout << "Are we done? ";
		auto still_going = !coro.done();
		return still_going;
	}

	int current_value() {
		return coro.promise().current_value;
	}
private:
	generator(handle_type h)
		: coro(h) {}
	handle_type coro;
};