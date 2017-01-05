#include "log_system.hpp"
#include "../../main/game_globals.hpp"
#include "../../messages/log_message.hpp"
#include "../../messages/tick_message.hpp"
#include <algorithm>
#include "../../external/imgui-sfml/imgui-SFML.h"

constexpr int MAX_LOG_AGE = 200;
constexpr int MAX_LOG_LINES = 10;
constexpr int LOG_TERM = 3;

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

    if (game_master_mode == PLAY) {
        if (first_run) {
            ImGui::SetNextWindowPos(ImVec2(5, get_window()->getSize().y - 105), ImGuiSetCond_Always);
            first_run = false;
        }
        ImGui::Begin("Log", nullptr, ImVec2{600,100}, ImGuiWindowFlags_AlwaysAutoResize);
        for (const auto &line : logger->lines) {
            std::string output;
            for (const rltk::vchar &c : line.chars) {
                output += c.glyph;
            }
            ImGui::Text("%s", output.c_str());
        }
        ImGui::End();
    }
}
