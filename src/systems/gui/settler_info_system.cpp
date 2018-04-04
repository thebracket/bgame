#include "stdafx.h"
#include "settler_info_system.hpp"
#include "units_info_system.hpp"
#include "../../bengine/IconsFontAwesome.h"
#include "../../bengine/imgui.h"
#include "../../global_assets/game_planet.hpp"
#include "../../raws/life_events.hpp"
#include "../../raws/defs/life_event_template.hpp"
#include "../../global_assets/game_mode.hpp"
#include "item_info_system.hpp"
#include "../../utils/system_log.hpp"
#include "../../bengine/btabs.hpp"

namespace systems {
	namespace settler_ui {

		static name_t * name;
		static game_stats_t * stats;
		static species_t * species;
		static settler_ai_t * ai;
		static health_t * health;

		static const std::string tab_summary = std::string(ICON_FA_USER) + " Summary";
		static const std::string tab_stats = std::string(ICON_FA_BAR_CHART) + " Stats";
		static const std::string tab_health = std::string(ICON_FA_HEART) + " Health";
		static const std::string tab_inventory = std::string(ICON_FA_SHIELD) + " Inventory";
		static const std::string tab_emotions = std::string(ICON_FA_SNOWFLAKE_O) + " Emotional";
		static const std::string tab_relations = std::string(ICON_FA_HANDSHAKE_O) + " Relationships";
		static const std::string tab_history = std::string(ICON_FA_HISTORY) + " History";
		static const std::string tab_schedule = std::string(ICON_FA_CALENDAR) + " Schedule";
		static const std::string tab_work = std::string(ICON_FA_CHECK) + " Labor";

		static inline void add_attribute_row(short attribute, std::string name, std::string desc) {
			ImGui::Text("%s", name.c_str());
			ImGui::NextColumn();
			ImGui::ProgressBar(static_cast<float>(attribute) / 20.0f);
			ImGui::SameLine();
			ImGui::Text("%d", attribute);
			ImGui::NextColumn();
			const std::string str = desc.c_str();
			ImGui::Text("%s", str.c_str());
			ImGui::NextColumn();
			ImGui::Separator();
		}

		static inline void render_stats() {
			ImGui::Columns(3, "stats_grid");
			ImGui::Separator();

			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Attribute"); ImGui::NextColumn();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Level"); ImGui::NextColumn();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Description"); ImGui::NextColumn();
			ImGui::Separator();

			add_attribute_row(stats->strength, "Strength", species->gender_pronoun() + std::string(" ") + stats->strength_str());
			add_attribute_row(stats->dexterity, "Dexterity", species->gender_pronoun() + std::string(" ") + stats->dexterity_str());
			add_attribute_row(stats->constitution, "Constitution", species->gender_pronoun() + std::string(" ") + stats->constitution_str());
			add_attribute_row(stats->intelligence, "Intelligence", species->gender_pronoun() + std::string(" ") + stats->intelligence_str());
			add_attribute_row(stats->wisdom, "Wisdom", species->gender_pronoun() + std::string(" ") + stats->wisdom_str());
			add_attribute_row(stats->charisma, "Charisma", species->gender_pronoun() + std::string(" ") + stats->charisma_str());
			add_attribute_row(stats->comeliness, "Comeliness", species->gender_pronoun() + std::string(" ") + stats->comeliness_str());
			add_attribute_row(stats->ethics, "Ethics", species->gender_pronoun() + std::string(" ") + stats->ethics_str());

			ImGui::Columns(1);

			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Skills");
			ImGui::Columns(3, "skills_grid");
			ImGui::Separator();

			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Skill"); ImGui::NextColumn();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Level"); ImGui::NextColumn();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Progress"); ImGui::NextColumn();
			ImGui::Separator();

			for (const auto &skill : stats->skills) {
				ImGui::Text("%s", skill.first.c_str());
				ImGui::NextColumn();

				ImGui::ProgressBar(static_cast<float>(skill.second.skill_level) / 10.0f);
				ImGui::NextColumn();

				ImGui::Text("%d", skill.second.experience_gained);
				ImGui::NextColumn();
			}

			ImGui::Columns(1);
		}

		static inline void render_summary() {
			fmt::MemoryWriter header;
			header << " " << name->first_name << " " << name->last_name << " (" << stats->profession_tag << ") ";
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", header.str().c_str());

			fmt::MemoryWriter header2;
			header2 << species->gender_str() << ", " << species->sexuality_str() << ", " << stats->age << " years old. " << species->height_feet() << ", " << species->weight_lbs();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "%s", header2.str().c_str());

			fmt::MemoryWriter profile;
			profile << species->gender_pronoun() << std::string(" has ") << species->ethnicity() << std::string(" colored skin. ");
			profile << species->gender_pronoun() << std::string(" has ") << species->hair_color_str() << std::string(" hair, ") << species->hair_style_str() << ".";
			ImGui::Text("%s", profile.str().c_str());


			ImGui::Text("Hit Points: "); ImGui::SameLine();
			ImGui::ProgressBar(static_cast<float>(health->current_hitpoints) / static_cast<float>(health->max_hitpoints));

			ImGui::Text("%s: %s", "Current Status:", ai->job_status.c_str());
		}

		static inline void render_health() {
			ImGui::Text("Hit Points: "); ImGui::SameLine();
			ImGui::ProgressBar(static_cast<float>(health->current_hitpoints) / static_cast<float>(health->max_hitpoints));

			ImGui::Columns(2, "health_grid");
			ImGui::Separator();

			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Part"); ImGui::NextColumn();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "State"); ImGui::NextColumn();
			ImGui::Separator();

			for (const health_part_t &part : health->parts) {
				std::string part_state = "OK";
				if (part.current_hitpoints < 1 && part.current_hitpoints > -10) part_state = "IMPAIRED";
				if (part.current_hitpoints < -9 && part.current_hitpoints > -20) part_state = "BROKEN";
				if (part.current_hitpoints < -19) part_state = "GONE";

				ImGui::Text("%s", part.part.c_str());
				ImGui::NextColumn();

				ImGui::Text("%s", part_state.c_str());
				ImGui::NextColumn();

				ImGui::Separator();
			}

			ImGui::Columns(1);
		}

		static inline void render_inventory() {
			using namespace bengine;

			ImGui::Columns(5, "inventory_grid");
			ImGui::Separator();

			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Item"); ImGui::NextColumn();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Quality"); ImGui::NextColumn();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Condition"); ImGui::NextColumn();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Location"); ImGui::NextColumn();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Options"); ImGui::NextColumn();
			ImGui::Separator();

			each_if<item_t, item_carried_t>( 
				[] (entity_t &e, item_t &item, item_carried_t &carried) {
					return carried.carried_by == units_ui::selected_settler;
				},
				[] (entity_t &e, item_t &item, item_carried_t &carried) {
					ImGui::Text("%s", item.item_name.c_str());
					ImGui::NextColumn();

					std::string qual = "";
					auto qc = e.component<item_quality_t>();
					if (qc) qual = qc->get_quality_text();
					ImGui::Text("%s", qual.c_str());
					ImGui::NextColumn();

					float wear = 1.0f;
					auto wc = e.component<item_wear_t>();
					if (wc) wear = static_cast<float>(wc->wear) / 100.0f;
					ImGui::ProgressBar(wear);
					ImGui::NextColumn();

					std::string ci = item_loc_name(carried.location);
					ImGui::Text("%s", ci.c_str());
					ImGui::NextColumn();

					const std::string btn_view = std::string(ICON_FA_CAMERA) + " View##" + std::to_string(e.id);
					if (ImGui::Button(btn_view.c_str())) {
						game_master_mode = ITEM_INFO;
						item_ui::selected_item = e.id;
					}
					ImGui::NextColumn();

					ImGui::Separator();
				}
			);
		}

		static std::vector<bengine::table_heading_t> life_headings{
			{ "Year", 50.0f },{ "Event", -1.0f }
		};
		static bool history_headings_first_run = true;

		static inline void render_history() {
			bengine::begin_table(history_headings_first_run, life_headings, "life_history_grid");

			bool zebra = true;
			for (const life_event_t &le : planet.history.settler_life_events[units_ui::selected_settler]) {
				const auto finder = get_life_event(le.type);
				if (finder != nullptr) {
					bengine::zebra_row(zebra);
					const std::string year = std::to_string(le.year);
					std::string event = finder->description + std::string(" ");

					bengine::begin_zebra_col(zebra);
					ImGui::Text("%s", year.c_str());
					bengine::end_zebra_col();

					bengine::begin_zebra_col(zebra);
					ImGui::Text("%s", event.c_str());
					bengine::end_zebra_col();
				}
				else {
					glog(log_target::GAME, log_severity::warning, "Warning: {0} life event not found", le.type);
				}
			}
		}

		static void render_placeholder()
		{
			ImGui::Text("Placeholder");
		}

		static bengine::btabs_t settler_tabs{
			{
				bengine::btab_t{ tab_summary, render_summary },
				bengine::btab_t{ tab_stats, render_stats },
				bengine::btab_t{ tab_health , render_health },
				bengine::btab_t{ tab_schedule , render_placeholder },
				bengine::btab_t{ tab_work , render_placeholder },
				bengine::btab_t{ tab_inventory , render_inventory },
				bengine::btab_t{ tab_emotions , render_placeholder },
				bengine::btab_t{ tab_relations , render_placeholder },
				bengine::btab_t{ tab_history , render_history },
			}
		};

		static bool show_window = true;

		void run(const double &duration_ms) {
			using namespace bengine;
			using namespace systems::units_ui;
			name = entity(selected_settler)->component<name_t>();
			stats = entity(selected_settler)->component<game_stats_t>();
			species = entity(selected_settler)->component<species_t>();
			ai = entity(selected_settler)->component<settler_ai_t>();
			health = entity(selected_settler)->component<health_t>();
			std::stringstream header;
			header << std::string(ICON_FA_USER) << " " << name->first_name << " " << name->last_name << " (" << stats->profession_tag << ") ";

			bengine::begin_info_window(header.str(), &show_window);
			bengine::render_btab_bar(settler_tabs);
			ImGui::End();

			if (!show_window) {
				game_master_mode = PLAY;
				show_window = true;
			}
		}
	}
}
