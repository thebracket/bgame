#include "telemetry.hpp"
#include "threadsafequeue.hpp"
#include "../main/game_globals.hpp"
#include "../main/constants.hpp"
#include <iostream>
#include <string>
#include <thread>
#include <memory>
#include <atomic>
#include <sstream>
#include <chrono>
#include <SFML/Network.hpp>

namespace telemetry {
    std::atomic<bool> quitting(false);
    bool allow_telemetry = true;
    std::string username = "anonymouse";
    concurrent_queue<telemetry_t> msg_queue;
    std::unique_ptr<std::thread> msg_thread;
    std::time_t session_stamp = std::time(nullptr);

    void message_loop() {
        do {
            std::this_thread::sleep_for(2s);
            telemetry_t msg;
            while (msg_queue.try_pop(msg)) {
                sf::Http::Request request("/telemetry-post.php", sf::Http::Request::Post);
                std::ostringstream stream;
                stream << "game=bf&session=" << session_stamp << "&version=" << VERSION << "&event=" << msg.event << "&details=" << msg.details;
                request.setBody(stream.str());
                sf::Http http("http://bfnightly.bracketproductions.com/");
                sf::Http::Response response = http.sendRequest(request);
                // We don't really care if it worked, ignore the result
            }
        } while (!quitting);
    }

    void report_startup() {
        if (allow_telemetry) {
            std::cout << "Starting telemetry system. Username is: " << username << "\n";
            msg_thread = std::make_unique<std::thread>(message_loop);
        } else {
            std::cout << "Skipping telemetry\n";
        }
    }

    void report_stop() {
        if (allow_telemetry) {
            std::cout << "Stopping telemetry\n";
        }
        if (msg_thread) {
            quitting = true;
            msg_thread->join();
        }
    }
}

void start_telemetry() {
    telemetry::allow_telemetry = game_config.allow_calling_home;
    telemetry::username = game_config.online_username;
    telemetry::report_startup();
}

void stop_telemetry() {
    telemetry::report_stop();
}

void call_home(const std::string &event, const std::string details) {
    if (telemetry::allow_telemetry) {
        telemetry::msg_queue.push(telemetry_t{event, details});
    }
}