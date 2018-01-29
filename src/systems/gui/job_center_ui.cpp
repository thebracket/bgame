#include "job_center_ui.hpp"
#include "stdafx.h"
#include "../../bengine/IconsFontAwesome.h"
#include "../../bengine/imgui.h"
#include "../../bengine/imgui_impl_glfw_gl3.h"
#include "../../bengine/imgui_tabs.hpp"
#include "../../components/settler_ai.hpp"
#include "../../components/name.hpp"
#include "../../components/game_stats.hpp"
#include "../../components/species.hpp"
#include "../../components/grazer_ai.hpp"
#include "../../components/sentient_ai.hpp"
#include "../../global_assets/game_mode.hpp"
#include "../../global_assets/game_camera.hpp"
#include "../../render_engine/camera.hpp"
#include "../../render_engine/vox/renderables.hpp"
#include "../../components/health.hpp"
#include "../../stdafx.h"
#include "../helpers/inventory_assistant.hpp"
#include "../../components/item_tags/item_digging_t.hpp"
#include "../../components/mining/designated_miner.hpp"
#include "../../components/designated_lumberjack.hpp"
#include "../../components/items/item_carried.hpp"
#include "../../components/item_tags/item_chopping_t.hpp"
#include "../ai/inventory_system.hpp"

namespace systems {
	namespace job_center_ui {

		static std::string win_job_center = std::string(ICON_FA_BRIEFCASE) + " Job Center";;
		static std::string mining_job_tab = std::string(ICON_FA_DIAMOND) + " Mining";
		static std::string lumberjacking_job_tab = std::string(ICON_FA_TREE) + " Lumberjacking";
		const static std::string btn_close = std::string(ICON_FA_TIMES) + " Close";

		static inline void render_miners() {
			using namespace bengine;

			ImGui::Columns(4, "miner_list_grid");
			ImGui::Separator();

			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Settler Name"); ImGui::NextColumn();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Profession"); ImGui::NextColumn();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Suitability"); ImGui::NextColumn();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Options"); ImGui::NextColumn();
			ImGui::Separator();

			each<settler_ai_t, name_t, game_stats_t, species_t, health_t>([](entity_t &e, settler_ai_t &ai, name_t &name, game_stats_t &stats, species_t &species, health_t &h) {
				auto miner_designation = e.component<designated_miner_t>();
				const std::string gender = (species.gender == MALE) ? std::string(ICON_FA_MALE) : std::string(ICON_FA_FEMALE);
				const std::string dname = name.first_name + std::string(" ") + name.last_name;
				const std::string profession = stats.profession_tag;
				const std::string task = ai.job_status;
				int mine_skill = 0;
				for (const auto & sk : stats.skills) {
					if (sk.first == "Mining") mine_skill = sk.second.skill_level;
				}

				if (!miner_designation) {
					ImGui::Text("%s %s", gender.c_str(), dname.c_str());
				}
				else {
					ImGui::TextColored(ImVec4{ 1.0f, 1.0f, 0.0f, 1.0f }, "%s %s", gender.c_str(), dname.c_str());
				}
				ImGui::NextColumn();
				ImGui::Text("%s", profession.c_str());
				ImGui::NextColumn();

				ImGui::Text("Str: %d, Dex: %d, HP: %d/%d, Skill: %d", stats.strength, stats.dexterity, h.current_hitpoints, h.max_hitpoints, mine_skill);
				ImGui::NextColumn();

				if (!miner_designation && e.component<designated_lumberjack_t>() == nullptr) {
					if (inventory::is_item_category_available<item_digging_t>()) {
						const std::string btn_makeminer = std::string(ICON_FA_DIAMOND) + std::string(" Make Miner##") + std::to_string(e.id);
						if (ImGui::Button(btn_makeminer.c_str())) {
							// TODO: Add happyness
							auto pos = e.component<position_t>();
							stats.profession_tag = "Miner";
							inventory::find_closest_unclaimed_item_by_category_and_claim_it_immediately<item_digging_t>(e.id, *pos);
							e.assign(designated_miner_t{});
						}
					}
				}
				else if (miner_designation) {
					const std::string btn_fireminer = std::string(ICON_FA_TIMES) + std::string(" Fire Miner##") + std::to_string(e.id);
					if (ImGui::Button(btn_fireminer.c_str())) {
						// TODO: Add unhappiness
						stats.profession_tag = stats.original_profession;
						delete_component<designated_miner_t>(e.id);

						// Drop miner items
						auto pos = e.component<position_t>();
						each<item_digging_t, item_carried_t>([&e, &pos](entity_t &E, item_digging_t &dig, item_carried_t &carried) {
							if (carried.carried_by == e.id) {
								systems::inventory_system::drop_item(E.id, pos->x, pos->y, pos->z);
							}
						});

						// Clear any claims
						std::vector<std::size_t> to_unclaim;
						each<item_digging_t, claimed_t>([&e, &to_unclaim](entity_t &E, item_digging_t &dig, claimed_t &claim) {
							if (claim.claimed_by == e.id) to_unclaim.push_back(E.id);
						});
						for (const auto &id : to_unclaim) {
							delete_component<claimed_t>(id);
						}
					}
				}

				ImGui::NextColumn();
				ImGui::Separator();
			});
		}

		static inline void render_lumberjacks() {
			using namespace bengine;

			ImGui::Columns(4, "lumberjack_list_grid");
			ImGui::Separator();

			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Settler Name"); ImGui::NextColumn();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Profession"); ImGui::NextColumn();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Suitability"); ImGui::NextColumn();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Options"); ImGui::NextColumn();
			ImGui::Separator();

			each<settler_ai_t, name_t, game_stats_t, species_t, health_t>([](entity_t &e, settler_ai_t &ai, name_t &name, game_stats_t &stats, species_t &species, health_t &h) {
				auto lj_designation = e.component<designated_lumberjack_t>();
				const std::string gender = (species.gender == MALE) ? std::string(ICON_FA_MALE) : std::string(ICON_FA_FEMALE);
				const std::string dname = name.first_name + std::string(" ") + name.last_name;
				const std::string profession = stats.profession_tag;
				const std::string task = ai.job_status;
				int mine_skill = 0;
				for (const auto & sk : stats.skills) {
					if (sk.first == "Lumberjacking") mine_skill = sk.second.skill_level;
				}

				if (!lj_designation) {
					ImGui::Text("%s %s", gender.c_str(), dname.c_str());
				}
				else {
					ImGui::TextColored(ImVec4{ 1.0f, 1.0f, 0.0f, 1.0f }, "%s %s", gender.c_str(), dname.c_str());
				}
				ImGui::NextColumn();
				ImGui::Text("%s", profession.c_str());
				ImGui::NextColumn();

				ImGui::Text("Str: %d, Dex: %d, HP: %d/%d, Skill: %d", stats.strength, stats.dexterity, h.current_hitpoints, h.max_hitpoints, mine_skill);
				ImGui::NextColumn();

				if (!lj_designation && e.component<designated_miner_t>()==nullptr) {
					if (inventory::is_item_category_available<item_chopping_t>()) {
						const std::string btn_makeminer = std::string(ICON_FA_TREE) + std::string(" Make Lumberjack##") + std::to_string(e.id);
						if (ImGui::Button(btn_makeminer.c_str())) {
							// TODO: Add happyness
							auto pos = e.component<position_t>();
							stats.profession_tag = "Lumberjack";
							inventory::find_closest_unclaimed_item_by_category_and_claim_it_immediately<item_chopping_t>(e.id, *pos);
							e.assign(designated_lumberjack_t{});
						}
					}
				}
				else if (lj_designation) {
					const std::string btn_fireminer = std::string(ICON_FA_TIMES) + std::string(" Fire Lumberjack##") + std::to_string(e.id);
					if (ImGui::Button(btn_fireminer.c_str())) {
						// TODO: Add unhappiness
						stats.profession_tag = stats.original_profession;
						delete_component<designated_lumberjack_t>(e.id);

						// Drop lj items
						auto pos = e.component<position_t>();
						each<item_chopping_t, item_carried_t>([&e, &pos](entity_t &E, item_chopping_t &dig, item_carried_t &carried) {
							if (carried.carried_by == e.id) {
								systems::inventory_system::drop_item(E.id, pos->x, pos->y, pos->z);
							}
						});

						// Clear any claims
						std::vector<std::size_t> to_unclaim;
						each<item_chopping_t, claimed_t>([&e, &to_unclaim](entity_t &E, item_chopping_t &dig, claimed_t &claim) {
							if (claim.claimed_by == e.id) to_unclaim.push_back(E.id);
						});
						for (const auto &id : to_unclaim) {
							delete_component<claimed_t>(id);
						}
					}
				}

				ImGui::NextColumn();
				ImGui::Separator();
			});
		}

		void run(const double &duration_ms) {
			ImGui::Begin(win_job_center.c_str());

			ImGui::SameLine();
			if (ImGui::Button(btn_close.c_str())) {
				game_master_mode = PLAY;
			}

			ImGui::BeginTabBar("##units#jobs_bar");
			ImGui::DrawTabsBackground();

			if (ImGui::AddTab(mining_job_tab.c_str())) {
				render_miners();
			}
			if (ImGui::AddTab(lumberjacking_job_tab.c_str())) {
				render_lumberjacks();
			}
			ImGui::EndTabBar();
			ImGui::End();
		}
	}
}