#include "telemetry.hpp"
#include "threadsafequeue.hpp"
#include "../global_assets/game_config.hpp"
#include "analytics.hpp"
#include "filesystem.hpp"
#include <boost/optional.hpp>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include <fstream>

static const char * tracking_id = "UA-44498023-6";

struct telemetry_t {
	telemetry_t() {}
	telemetry_t(std::string cat, std::string act) : category(cat), action(act) {}
	telemetry_t(std::string cat, std::string act, std::string lab) : category(cat), action(act), label(lab) {}
	telemetry_t(std::string cat, std::string act, std::string lab, uint32_t val) : category(cat), action(act), label(lab), value(val) {}

    std::string category = "";
    std::string action = "";
	boost::optional<std::string> label;
	boost::optional<uint32_t> value;
};

namespace telemetry {
    std::atomic<bool> quitting(false);
    bool allow_telemetry = true;
    std::string username = "anonymouse";
    concurrent_queue<telemetry_t> msg_queue;
    std::unique_ptr<std::thread> msg_thread;
    std::time_t session_stamp = std::time(nullptr);

    void message_loop() {
        do {
			using namespace std::chrono_literals;
            std::this_thread::sleep_for(2s);
            telemetry_t msg;
            while (msg_queue.try_pop(msg)) {
				// Send it
				if (msg.label) {
					if (msg.value) {
						bengine::analytics::on_event(msg.category.c_str(), msg.action.c_str(), msg.label.get().c_str(), msg.value.get());
					}
					else {
						bengine::analytics::on_event(msg.category.c_str(), msg.action.c_str(), msg.label.get().c_str());
					}
				}
				else {
					bengine::analytics::on_event(msg.category.c_str(), msg.action.c_str());
				}
            }
        } while (!quitting);
    }

	std::string get_guid() {
		std::string guid = "";

		std::string guid_path = get_save_path() + "/userid.conf";
		if (exists(guid_path)) {
			// Load it
			std::ifstream f(guid_path);
			std::string line;
			while (getline(f, line))
			{
				if (!line.empty()) guid = line;
			}
		}
		else {
			// Create it
			boost::uuids::uuid uuid = boost::uuids::random_generator()();
			guid = boost::uuids::to_string(uuid);
			std::ofstream f(guid_path);
			f << guid << "\n";
		}

		return guid;
	}

    void report_startup() {
        if (allow_telemetry) {
            msg_thread = std::make_unique<std::thread>(message_loop);
			std::string guid = get_guid();
			//std::cout << "GUID: " << guid << "\n";
			bengine::analytics::init(tracking_id, guid.c_str());
        }
    }

    void report_stop() {
        if (msg_thread) {
            quitting = true;
            msg_thread->join();
            msg_thread.reset();
			bengine::analytics::shutdown();
        }
    }

	void on_tick() {
		bengine::analytics::update_tick();
	}
}

void start_telemetry() {
    telemetry::allow_telemetry = config::game_config.allow_calling_home;
    telemetry::username = config::game_config.online_username;
    telemetry::report_startup();
}

void stop_telemetry() {
    telemetry::report_stop();
}

void call_home(const std::string &category, const std::string &action) {
	if (!telemetry::allow_telemetry) return;
	telemetry::msg_queue.push(telemetry_t{ category, action });
}

void call_home(const std::string &category, const std::string &action, const std::string &label) {
	if (!telemetry::allow_telemetry) return;
	telemetry::msg_queue.push(telemetry_t{ category, action, label });
}

void call_home(const std::string &category, const std::string &action, const std::string &label, const uint32_t &value) {
	if (!telemetry::allow_telemetry) return;
	telemetry::msg_queue.push(telemetry_t{ category, action, label, value });
}
