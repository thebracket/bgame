#include "log_system.hpp"
#include "../main/game_globals.hpp"
#include "../messages/log_message.hpp"
#include "../messages/tick_message.hpp"
#include <algorithm>

constexpr int MAX_LOG_AGE = 200;
constexpr int MAX_LOG_LINES = 10;

void log_system::configure() {
    system_name = "Log System";
    subscribe_mbox<log_message>();

    subscribe<tick_message>([] (tick_message &msg) {
        for (auto &line : logger->lines) {
            ++line.age;
        }
    });
}

void log_system::update(const double ms) {
    std::queue<log_message> * log = mbox<log_message>();
    while (!log->empty()) {
        log_message msg = log->front();
        log->pop();

        logger->lines.insert(logger->lines.begin(), log_line_t{msg.chars});
    }

    // Cull older log entries
    logger->lines.erase(std::remove_if(logger->lines.begin(), logger->lines.end(), [] (auto &l) { return l.age>MAX_LOG_AGE; }), logger->lines.end());

    // Render on screen for now
    if (game_master_mode == PLAY) {
        const int max_x = term(1)->term_width;
        const int max_y = term(1)->term_height;

        int Y = max_y - 2;
        int i=0;
        for (const auto &line : logger->lines) {
            if (i < MAX_LOG_LINES) {
                int X = 2;

                for (const rltk::vchar &c : line.chars) {
                    term(4)->set_char(X, Y, c);
                    ++X;
                    if (X > (max_x -1)) {
                        X = 6;
                        --Y;
                    }
                }
                --Y;

            } else {
                break;
            }
            ++i;
        }
    }
}
