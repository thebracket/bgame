#include "telemetry.hpp"
#include "threadsafequeue.hpp"
#include "../global_assets/game_config.hpp"
#include "../global_assets/constants.hpp"
#include <iostream>
#include <curl/curl.h>
#include <atomic>
#include <chrono>
#include <ctime>
#include <sstream>

struct telemetry_t {
    std::string event;
    std::string details;
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
            std::this_thread::sleep_for(2s);
            telemetry_t msg;
            while (msg_queue.try_pop(msg)) {
                // Populate the body
                std::ostringstream ss;
                ss << "game=bf&user=" << username << "&session=" << session_stamp << "&version=" << VERSION << "&event=" << msg.event << "&details=" << msg.details;

                CURL *curl;
                CURLcode res;

                curl_global_init(CURL_GLOBAL_ALL);
                curl = curl_easy_init();
                if (curl) {
                    std::string body = ss.str();
                    curl_easy_setopt(curl, CURLOPT_URL, "http://bfnightly.bracketproductions.com/telemetry-post.php");
                    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
                    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, -1L);
                    struct curl_slist *headers = nullptr;
                    curl_slist_append(headers, "Expect:");
                    curl_slist_append(headers, "Content-Type: application/json");
                    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
                    res = curl_easy_perform(curl);
                    if (res != CURLE_OK) fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
                    curl_easy_cleanup(curl);
                }
                curl_global_cleanup();

                /*
                sf::Http::Request request("/telemetry-post.php", sf::Http::Request::Post);
                std::ostringstream stream;
                stream << "game=bf&user=" << username << "&session=" << session_stamp << "&version=" << VERSION << "&event=" << msg.event << "&details=" << msg.details;
                request.setBody(stream.str());
                sf::Http http("http://bfnightly.bracketproductions.com/");
                sf::Http::Response response = http.sendRequest(request);
                // We don't really care if it worked, ignore the result
                 */


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
            msg_thread.reset();
        }
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

void call_home(const std::string &event, const std::string details) {
    if (telemetry::allow_telemetry) {
        telemetry::msg_queue.push(telemetry_t{event, details});
    }
}