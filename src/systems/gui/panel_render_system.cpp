#include "panel_render_system.hpp"
#include "../../main/game_globals.hpp"
#include "../../raws/raws.hpp"
#include "../../raws/reactions.hpp"
#include "../../messages/messages.hpp"
#include "../ai/mining_system.hpp"
#include "../ai/inventory_system.hpp"
#include "../render/camera_system.hpp"
#include "../../utils/string_utils.hpp"
#include "../input/mouse_input_system.hpp"
#include "../../external/imgui-sfml/imgui-SFML.h"
#include "imgui_helper.hpp"
#include <sstream>
#include <iomanip>
#include <map>

using namespace rltk;
using namespace rltk::colors;

const color_t GREEN_BG{0,32,0};

void panel_render_system::update(const double duration_ms) {
	term(2)->clear();

	mouse_damper += duration_ms;

	term(3)->clear();

	render_mode_select(duration_ms);
}

void panel_render_system::configure() {
	system_name = "Panel Render";	
}

void panel_render_system::render_mode_select(const double duration_ms) {
	switch (game_master_mode) {
		case WORKFLOW : render_work_mode(); break;
		case ROGUE : {
			term(2)->print(28,1,"ESC", YELLOW);
			term(2)->print(32,1,"Return to normal play", WHITE, GREEN_BG);
		} break;
	}
}

void panel_render_system::render_work_mode()
{
    std::vector<std::string> worklist;
    for (const std::pair<uint8_t, std::string> &order : designations->build_orders) {
        auto finder = reaction_defs.find(order.second);
        if (finder != reaction_defs.end()) {
            worklist.emplace_back(std::to_string(order.first) + std::string(" ") + finder->second.name);
        }
    }
    const char* work_listbox_items[worklist.size()];
    for (int i=0; i<worklist.size(); ++i) {
        work_listbox_items[i] = worklist[i].c_str();
    }

    ImGui::Begin(win_workflow.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);
    ImGui::ListBox("Queued Jobs", &selected_work_item, work_listbox_items, worklist.size(), 10);

    std::vector<std::string> buildlist;
    auto available_reactions = get_available_reactions();
    for (const auto &reaction : available_reactions) {
        buildlist.emplace_back(reaction.second);
    }
    const char* buildable_listbox_items[buildlist.size()];
    for (int i=0; i<buildlist.size(); ++i) {
        buildable_listbox_items[i] = buildlist[i].c_str();
    }

    ImGui::ListBox("Available Jobs", &selected_build_item, buildable_listbox_items, buildlist.size(), 10);
    if (ImGui::Button(btn_build.c_str())) {
        const std::string tag = available_reactions[selected_build_item].first;
        bool found = false;
        for (auto &order : designations->build_orders) {
            if (order.second == tag) {
                ++order.first;
                found = true;
            }
        }
        if (!found) designations->build_orders.push_back(std::make_pair(1, tag));
    }
    ImGui::SameLine();
    if (ImGui::Button(btn_close.c_str())) {
        game_master_mode = PLAY;
        emit_deferred(map_dirty_message{});
        emit_deferred(recalculate_mining_message{});
    }

    ImGui::End();
}
