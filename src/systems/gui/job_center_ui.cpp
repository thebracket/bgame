#include "stdafx.h"
#include "job_center_ui.hpp"
#include "../../bengine/IconsFontAwesome.h"
#include "../../bengine/imgui.h"
#include "../../global_assets/game_mode.hpp"
#include "../helpers/inventory_assistant.hpp"
#include "../ai/inventory_system.hpp"
#include "../../bengine/btabs.hpp"

namespace systems {
	namespace job_center_ui {

		static const std::string win_job_center = std::string(ICON_FA_BRIEFCASE) + " Job Center";;
		static const std::string mining_job_tab = std::string(ICON_FA_DIAMOND) + " Mining";
		static const std::string lumberjacking_job_tab = std::string(ICON_FA_TREE) + " Lumberjacking";
		static const std::string farming_job_tab = std::string(ICON_FA_LEAF) + " Farming";
		static const std::string hunting_job_tab = std::string(ICON_FA_BULLSEYE) + " Hunting";
		const static std::string btn_close = std::string(ICON_FA_TIMES) + " Close";

		static bool miner_headings_first_run = true;
		static bool lumberjack_headings_first_run = true;
		static bool farmers_headings_first_run = true;
		static bool hunters_headings_first_run = true;
		static std::vector<bengine::table_heading_t> jcui_headings{
			{ "Settler Name", 150.0f },{ "Profession", -1.0f }, { "Suitability", -1.0f}, {"Options", -1.0f}
		};

		static inline void render_miners() {
			using namespace bengine;

			bengine::begin_table(miner_headings_first_run, jcui_headings, "miners_list_grid");

			bool zebra = true;
			each<settler_ai_t, name_t, game_stats_t, species_t, health_t>([&zebra](entity_t &e, settler_ai_t &ai, name_t &name, game_stats_t &stats, species_t &species, health_t &h) {
				bengine::zebra_row(zebra);
				auto miner_designation = e.component<designated_miner_t>();
				const std::string gender = (species.gender == MALE) ? std::string(ICON_FA_MALE) : std::string(ICON_FA_FEMALE);
				const std::string dname = name.first_name + std::string(" ") + name.last_name;
				const std::string profession = stats.profession_tag;
				const std::string task = ai.job_status;
				int mine_skill = 0;
				for (const auto & sk : stats.skills) {
					if (sk.first == "Mining") mine_skill = sk.second.skill_level;
				}

				bengine::begin_zebra_col(zebra);
				if (!miner_designation) {
					ImGui::Text("%s %s", gender.c_str(), dname.c_str());
				}
				else {
					ImGui::TextColored(ImVec4{ 1.0f, 1.0f, 0.0f, 1.0f }, "%s %s", gender.c_str(), dname.c_str());
				}
				bengine::end_zebra_col();
				bengine::begin_zebra_col(zebra); 
				ImGui::Text("%s", profession.c_str());
				bengine::end_zebra_col();

				bengine::begin_zebra_col(zebra);
				ImGui::Text("Str: %d, Dex: %d, HP: %d/%d, Skill: %d", stats.strength, stats.dexterity, h.current_hitpoints, h.max_hitpoints, mine_skill);
				bengine::end_zebra_col();

				bengine::begin_zebra_col(zebra);
				if (!miner_designation && e.component<designated_lumberjack_t>() == nullptr && e.component<designated_farmer_t>() == nullptr && e.component<designated_hunter_t>() == nullptr) {
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

				bengine::end_zebra_col();
			});
			bengine::end_table();
		}

		static inline void render_lumberjacks() {
			using namespace bengine;

			bengine::begin_table(lumberjack_headings_first_run, jcui_headings, "lumberjack_list_grid");

			bool zebra = true;
			each<settler_ai_t, name_t, game_stats_t, species_t, health_t>([&zebra](entity_t &e, settler_ai_t &ai, name_t &name, game_stats_t &stats, species_t &species, health_t &h) {
				bengine::zebra_row(zebra);
				auto lj_designation = e.component<designated_lumberjack_t>();
				const std::string gender = (species.gender == MALE) ? std::string(ICON_FA_MALE) : std::string(ICON_FA_FEMALE);
				const std::string dname = name.first_name + std::string(" ") + name.last_name;
				const std::string profession = stats.profession_tag;
				const std::string task = ai.job_status;
				int mine_skill = 0;
				for (const auto & sk : stats.skills) {
					if (sk.first == "Lumberjacking") mine_skill = sk.second.skill_level;
				}

				bengine::begin_zebra_col(zebra);
				if (!lj_designation) {
					ImGui::Text("%s %s", gender.c_str(), dname.c_str());
				}
				else {
					ImGui::TextColored(ImVec4{ 1.0f, 1.0f, 0.0f, 1.0f }, "%s %s", gender.c_str(), dname.c_str());
				}
				bengine::end_zebra_col();
				bengine::begin_zebra_col(zebra);
				ImGui::Text("%s", profession.c_str());
				bengine::end_zebra_col();

				bengine::begin_zebra_col(zebra);
				ImGui::Text("Str: %d, Dex: %d, HP: %d/%d, Skill: %d", stats.strength, stats.dexterity, h.current_hitpoints, h.max_hitpoints, mine_skill);
				bengine::end_zebra_col();

				bengine::begin_zebra_col(zebra);
				if (!lj_designation && e.component<designated_miner_t>()==nullptr && e.component<designated_farmer_t>() == nullptr && e.component<designated_hunter_t>() == nullptr) {
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

				bengine::end_zebra_col();
			});
		}

		static inline void render_farmers() {
			using namespace bengine;

			bengine::begin_table(farmers_headings_first_run, jcui_headings, "farmers_list_grid");

			bool zebra = true;
			each<settler_ai_t, name_t, game_stats_t, species_t, health_t>([&zebra](entity_t &e, settler_ai_t &ai, name_t &name, game_stats_t &stats, species_t &species, health_t &h) {
				bengine::zebra_row(zebra);
				auto farm_designation = e.component<designated_farmer_t>();
				const std::string gender = (species.gender == MALE) ? std::string(ICON_FA_MALE) : std::string(ICON_FA_FEMALE);
				const std::string dname = name.first_name + std::string(" ") + name.last_name;
				const std::string profession = stats.profession_tag;
				const std::string task = ai.job_status;
				int mine_skill = 0;
				for (const auto & sk : stats.skills) {
					if (sk.first == "Farming") mine_skill = sk.second.skill_level;
				}

				bengine::begin_zebra_col(zebra);
				if (!farm_designation) {
					ImGui::Text("%s %s", gender.c_str(), dname.c_str());
				}
				else {
					ImGui::TextColored(ImVec4{ 1.0f, 1.0f, 0.0f, 1.0f }, "%s %s", gender.c_str(), dname.c_str());
				}
				bengine::end_zebra_col();
				bengine::begin_zebra_col(zebra);
				ImGui::Text("%s", profession.c_str());
				bengine::end_zebra_col();
				bengine::begin_zebra_col(zebra);
				ImGui::Text("Dex: %d, Wis: %d, HP: %d/%d, Skill: %d", stats.dexterity, stats.wisdom, h.current_hitpoints, h.max_hitpoints, mine_skill);
				bengine::end_zebra_col();
				bengine::begin_zebra_col(zebra);
				if (!farm_designation && e.component<designated_miner_t>() == nullptr && e.component<designated_lumberjack_t>() == nullptr && e.component<designated_hunter_t>() == nullptr) {
					if (inventory::is_item_category_available<item_farming_t>()) {
						const std::string btn_makeminer = std::string(ICON_FA_LEAF) + std::string(" Make Farmer##") + std::to_string(e.id);
						if (ImGui::Button(btn_makeminer.c_str())) {
							// TODO: Add happyness
							auto pos = e.component<position_t>();
							stats.profession_tag = "Farmer";
							inventory::find_closest_unclaimed_item_by_category_and_claim_it_immediately<item_farming_t>(e.id, *pos);
							e.assign(designated_farmer_t{});
						}
					}
				}
				else if (farm_designation) {
					const std::string btn_fireminer = std::string(ICON_FA_TIMES) + std::string(" Fire Farmer##") + std::to_string(e.id);
					if (ImGui::Button(btn_fireminer.c_str())) {
						// TODO: Add unhappiness
						stats.profession_tag = stats.original_profession;
						delete_component<designated_farmer_t>(e.id);

						// Drop lj items
						auto pos = e.component<position_t>();
						each<item_farming_t, item_carried_t>([&e, &pos](entity_t &E, item_farming_t &farm, item_carried_t &carried) {
							if (carried.carried_by == e.id) {
								systems::inventory_system::drop_item(E.id, pos->x, pos->y, pos->z);
							}
						});

						// Clear any claims
						std::vector<std::size_t> to_unclaim;
						each<item_farming_t, claimed_t>([&e, &to_unclaim](entity_t &E, item_farming_t &farm, claimed_t &claim) {
							if (claim.claimed_by == e.id) to_unclaim.push_back(E.id);
						});
						for (const auto &id : to_unclaim) {
							delete_component<claimed_t>(id);
						}
					}
				}

				bengine::end_zebra_col();
			});
		}

		void render_hunters()
		{
			using namespace bengine;

			bengine::begin_table(hunters_headings_first_run, jcui_headings, "hunters_list_grid");

			bool zebra = true;
			each<settler_ai_t, name_t, game_stats_t, species_t, health_t>([&zebra](entity_t &e, settler_ai_t &ai, name_t &name, game_stats_t &stats, species_t &species, health_t &h) {
				bengine::zebra_row(zebra);
				auto hunt_designation = e.component<designated_hunter_t>();
				const std::string gender = (species.gender == MALE) ? std::string(ICON_FA_MALE) : std::string(ICON_FA_FEMALE);
				const std::string dname = name.first_name + std::string(" ") + name.last_name;
				const std::string profession = stats.profession_tag;
				const std::string task = ai.job_status;
				int mine_skill = 0;
				for (const auto & sk : stats.skills) {
					if (sk.first == "Farming") mine_skill = sk.second.skill_level;
				}

				bengine::begin_zebra_col(zebra);
				if (!hunt_designation) {
					ImGui::Text("%s %s", gender.c_str(), dname.c_str());
				}
				else {
					ImGui::TextColored(ImVec4{ 1.0f, 1.0f, 0.0f, 1.0f }, "%s %s", gender.c_str(), dname.c_str());
				}
				bengine::end_zebra_col();
				bengine::begin_zebra_col(zebra);
				ImGui::Text("%s", profession.c_str());
				bengine::end_zebra_col();
				bengine::begin_zebra_col(zebra);
				ImGui::Text("Dex: %d, Wis: %d, HP: %d/%d, Skill: %d", stats.dexterity, stats.wisdom, h.current_hitpoints, h.max_hitpoints, mine_skill);
				bengine::end_zebra_col();
				bengine::begin_zebra_col(zebra);
				if (!hunt_designation && e.component<designated_miner_t>() == nullptr && e.component<designated_lumberjack_t>() == nullptr && e.component<designated_hunter_t>() == nullptr) {
					const std::string btn_makeminer = std::string(ICON_FA_BULLSEYE) + std::string(" Make Hunter##") + std::to_string(e.id);
					if (ImGui::Button(btn_makeminer.c_str())) {
						// TODO: Add happyness
						stats.profession_tag = "Hunter";
						e.assign(designated_hunter_t{});
					}
				}
				else if (hunt_designation) {
					const std::string btn_fireminer = std::string(ICON_FA_TIMES) + std::string(" Fire Hunter##") + std::to_string(e.id);
					if (ImGui::Button(btn_fireminer.c_str())) {
						// TODO: Add unhappiness
						stats.profession_tag = stats.original_profession;
						delete_component<designated_hunter_t>(e.id);
					}
				}

				bengine::end_zebra_col();
			});
		}

		static bengine::btabs_t jc_tabs{
			{
				bengine::btab_t{ mining_job_tab, render_miners },
				bengine::btab_t{ lumberjacking_job_tab, render_lumberjacks },
				bengine::btab_t{ farming_job_tab, render_farmers },
				bengine::btab_t{ hunting_job_tab, render_hunters },
			}
		};

		static bool show_window = true;

		void run(const double &duration_ms) {
			bengine::begin_info_window(win_job_center, &show_window);
			bengine::render_btab_bar(jc_tabs);
			ImGui::End();

			if (!show_window) {
				game_master_mode = PLAY;
				show_window = true;
			}
		}
	}
}
