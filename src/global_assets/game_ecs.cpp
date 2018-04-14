#include "stdafx.h"
#include <fstream>
#include <cereal/types/unordered_set.hpp>
#include <cereal/types/array.hpp>
#include <cereal/archives/portable_binary.hpp>

template<class Archive>
void serialize(Archive & archive, mining_designations_t &m)
{
	archive(m.mining_targets, m.brush_type, m.brush_size_x, m.brush_size_y, m.mine_mode, m.radius, m.stairs_helper, m.stairs_depth);
}

template<class Archive>
void serialize(Archive & archive, position_t &p)
{
	archive(p.x, p.y, p.z, p.offset_x, p.offset_y, p.offset_z, p.rotation); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, unbuild_t &u)
{
	archive(u.is_building, u.building_id); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, designations_t &d)
{
	archive(d.chopping, d.guard_points, d.deconstructions,
		d.levers_to_pull, d.current_power, d.current_cash, d.standing_order_idle_move, d.standing_order_wildlife_treatment,
		d.standing_order_upgrade); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, farm_cycle_t &f)
{
	archive(f.state, f.seed_type, f.days_since_weeded, f.days_since_watered, f.fertilized, f.seed_id);
}

template<class Archive>
void serialize(Archive & archive, farming_designations_t &f)
{
	archive(f.harvest, f.farms);
}

template<class Archive>
void serialize(Archive & archive, ai_tag_work_farm_plant &f)
{
	// Nothing to save
	archive(f.step, f.tool_id, f.seed_id, f.farm_position);
}

template<class Archive>
void serialize(Archive & archive, ai_tag_work_guarding &g)
{
	// Nothing to save
	archive(g.step, g.has_post, g.guard_post);
}

template<class Archive>
void serialize(Archive & archive, ai_mode_idle_t &tag)
{
	// Nothing to save
	archive(tag.dummy);
}

template<class Archive>
void serialize(Archive & archive, ai_settler_new_arrival_t &s)
{
	archive(s.turns_since_arrival);
}

template<class Archive>
void serialize(Archive & archive, ai_tag_leisure_shift_t &tag)
{
	// Nothing to save
	archive(tag.dummy);
}

template<class Archive>
void serialize(Archive & archive, ai_tag_leisure_eat_t &tag)
{
	// Nothing to save
	archive(tag.tool_id, tag.step);
}

template<class Archive>
void serialize(Archive & archive, ai_tag_leisure_drink_t &tag)
{
	// Nothing to save
	archive(tag.tool_id, tag.step);
}

template<class Archive>
void serialize(Archive & archive, ai_tag_my_turn_t &tag)
{
	// Nothing to save
	//archive( );
}

template<class Archive>
void serialize(Archive & archive, ai_tag_sleep_shift_t &tag)
{
	// Nothing to save
	//archive( );
}


template<class Archive>
void serialize(Archive & archive, ai_tag_work_architect &a)
{
	// Nothing to save
	archive(a.step, a.current_tool, a.target_tile);
}


template<class Archive>
void serialize(Archive & archive, ai_tag_work_building &b)
{
	// Nothing to save
	archive(b.step, b.building_target, b.current_tool);
}


template<class Archive>
void serialize(Archive & archive, ai_tag_work_butcher &b)
{
	// Nothing to save
	archive(b.step, b.target_corpse);
}


template<class Archive>
void serialize(Archive & archive, ai_tag_work_farm_clear &f)
{
	// Nothing to save
	archive(f.step, f.tool_id);
}


template<class Archive>
void serialize(Archive & archive, ai_tag_work_farm_fertilize &f)
{
	// Nothing to save
	archive(f.step, f.tool_id, f.fertilizer_id);
}


template<class Archive>
void serialize(Archive & archive, ai_tag_work_farm_fixsoil &f)
{
	// Nothing to save
	archive(f.step, f.tool_id, f.soil_id);
}


template<class Archive>
void serialize(Archive & archive, ai_tag_work_farm_water &f)
{
	// Nothing to save
	archive(f.step, f.tool_id);
}


template<class Archive>
void serialize(Archive & archive, ai_tag_work_farm_weed &tag)
{
	// Nothing to save
	archive(tag.step, tag.tool_id);
}


template<class Archive>
void serialize(Archive & archive, ai_tag_work_harvest &f)
{
	// Nothing to save
	archive(f.step, f.tool_id);
}


template<class Archive>
void serialize(Archive & archive, ai_tag_work_hunting &h)
{
	// Nothing to save
	archive(h.step);
}


template<class Archive>
void serialize(Archive & archive, ai_tag_work_lumberjack &l)
{
	// Nothing to save
	archive(l.step, l.tool_id, l.target_tree, l.target_x, l.target_y, l.target_z);
}


template<class Archive>
void serialize(Archive & archive, ai_tag_work_miner &m)
{
	// Nothing to save
	archive(m.step, m.tool_id, m.target_tile);
}

template<class Archive>
void serialize(Archive & archive, reaction_task_t &r)
{
	archive(r.building_id, r.job_name, r.reaction_tag, r.components); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, ai_tag_work_order & o)
{
	// Nothing to save
	archive(o.step, o.reaction_target, o.current_tool);
}

template<class Archive>
void serialize(Archive & archive, ai_tag_work_pull_lever &l)
{
	// Nothing to save
	archive(l.step);
}

template<class Archive>
void serialize(Archive & archive, ai_tag_work_shift_t &tag)
{
	// Nothing to save
	//archive( );
}


template<class Archive>
void serialize(Archive & archive, architecture_designations_t &a)
{
	archive(a.architecture); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, bridge_t &b)
{
	archive(b.complete, b.retracted); // serialize things by passing them to the archive
}


template<class Archive>
void serialize(Archive & archive, building_t &b)
{
	archive(b.tag, b.width, b.height, b.glyphs, b.glyphs_ascii, b.complete, b.built_with, b.civ_owner, b.max_hit_points, b.hit_points, b.vox_model); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, building_designations_t &b)
{
	archive(b.buildings, b.build_orders, b.standing_build_orders); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, construct_container_t &tag)
{
	//archive(  ); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, construct_power_t &p)
{
	archive(p.storage_capacity, p.generation_always, p.generation_solar); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, construct_door_t &d)
{
	archive(d.locked); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, construct_support_t &d)
{
	//archive(d.locked); // serialize things by passing them to the archive
}


template<class Archive>
void serialize(Archive & archive, construct_provides_sleep_t &tag)
{
	// Nothing to do here
}

template<class Archive>
void serialize(Archive & archive, entry_trigger_t &t)
{
	archive(t.active, t.type); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, receives_signal_t &s)
{
	archive(s.active, s.receives_from); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, smoke_emitter_t &s)
{
	//archive(  ); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, turret_t &t)
{
	archive(t.range, t.hit_bonus, t.damage_dice, t.damage_die, t.damage_bonus, t.owner_civilization); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, designated_farmer_t &tag)
{
	// Nothing to save
	//archive( );
}

template<class Archive>
void serialize(Archive & archive, designated_hunter_t &tag)
{
	// Nothing to save
	//archive( );
}

template<class Archive>
void serialize(Archive & archive, building_designation_t &b)
{
	archive(b.x, b.y, b.z, b.component_ids, b.name, b.tag, b.components, b.width, b.height, b.glyphs, b.building_entity, b.glyphs_ascii); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, shift_t &s)
{
	archive(s.shift_name, s.hours); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, calendar_t &c)
{
	archive(c.year, c.month, c.day, c.hour, c.minute, c.second, c.defined_shifts); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, camera_options_t &c)
{
	archive(c.camera_mode, c.ascii_mode, c.zoom_level, c.perspective);
}

template<class Archive>
void serialize(Archive & archive, claimed_t &c)
{
	archive(c.claimed_by);
}

template<class Archive>
void serialize(Archive & archive, corpse_harvestable &c)
{
	archive(c.ticks_since_death, c.creature_tag, c.claimed); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, corpse_settler &c)
{
	archive(c.ticks_since_death, c.cause_of_death); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, designated_lumberjack_t &tag)
{
	// Nothing to save
	//archive( );
}

template<class Archive>
void serialize(Archive & archive, explosion_t &e)
{
	archive(e.blast_radius, e.blast_timer, e.fuse_timer, e.damage_dice, e.damage_dice_type, e.tiles_hit); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, falling_t &f)
{
	archive(f.distance); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, skill_t &s)
{
	archive(s.skill_level, s.experience_gained); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, game_stats_t &s)
{
	archive(s.original_profession, s.profession_tag, s.strength, s.dexterity, s.constitution, s.intelligence, s.wisdom, s.charisma,
		s.comeliness, s.ethics, s.age, s.skills); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, grazer_ai &g)
{
	archive(g.dummy); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, health_part_t &h)
{
	archive(h.part, h.max_hitpoints, h.current_hitpoints, h.size); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, health_t &h)
{
	archive(h.max_hitpoints, h.current_hitpoints, h.unconscious, h.blind, h.immobile, h.slow, h.no_grasp, h.parts, h.stunned_counter); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, hunger_t &h)
{
	archive(h.hunger_clock); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, thirst_t &h)
{
	archive(h.thirst_clock); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, initiative_t &i)
{
	archive(i.initiative, i.initiative_modifier); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, node_graph_position_t &l)
{
	archive(l.x, l.y); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, lever_t &l)
{
	archive(l.active); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, oscillator_t &l)
{
	archive(l.active, l.interval, l.ticker); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, float_gauge_t &l)
{
	archive(l.active, l.water_level); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, pressure_plate_t &l)
{
	archive(l.active, l.include_friendlies); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, proximity_sensor_t &l)
{
	archive(l.active, l.range); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, sends_signal_t &l)
{
	archive(l.active, l.targets); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, signal_processor_t &l)
{
	archive(l.active, l.processor); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, lightsource_t &l)
{
	archive(l.radius, l.color, l.alert_status); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, log_line_t &l)
{
	archive(l.age, l.chars); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, logger_t &l)
{
	archive(l.lines); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, name_t &n)
{
	archive(n.first_name, n.last_name); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, natural_attack_t &a)
{
	archive(a.type, a.hit_bonus, a.n_dice, a.die_type, a.die_mod, a.range);
}

template<class Archive>
void serialize(Archive & archive, natural_attacks_t &a)
{
	archive(a.attacks);
}

template<class Archive>
void serialize(Archive & archive, renderable_t &r)
{
	archive(r.glyph, r.glyph_ascii, r.foreground, r.background, r.vox); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, renderable_composite_t &r)
{
	archive(r.render_mode, r.ascii_char); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, riding_t &r)
{
	archive(r.riding); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, sentient_ai &s)
{
	archive(s.aggression, s.civ_id, s.hostile, s.goal, s.target, s.days_since_arrival); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, settler_ai_t &s)
{
	archive(s.permitted_work, s.shift_id, s.job_type_major, s.job_type_minor, s.target_x,
		s.target_y, s.target_z, s.target_id, s.current_tool);
}

template<class Archive>
void serialize(Archive & archive, sleep_clock_t &s)
{
	archive(s.sleep_requirement, s.is_sleeping);
}

template<class Archive>
void serialize(Archive & archive, slidemove_t &s)
{
	archive(s.offsetX, s.offsetY, s.offsetZ, s.lifespan); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, species_t &s)
{
	archive(s.tag, s.gender, s.sexuality, s.hair_color, s.hair_style, s.skin_color, s.height_cm, s.weight_kg, s.bearded, s.index, s.base_male_glyph, s.base_female_glyph); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, stockpile_t &s)
{
	archive(s.category); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, viewshed_t &v)
{
	archive(v.viewshed_radius, v.penetrating, v.good_guy_visibility, v.visible_entities); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, water_spawner_t &w)
{
	archive(w.spawner_type); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, wildlife_group &w)
{
	archive(w.group_id); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, world_position_t &w)
{
	archive(w.world_x, w.world_y, w.region_x, w.region_y, w.region_z); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, item_ammo_t &tag)
{
	// Nothing to save
	//archive( );
}

template<class Archive>
void serialize(Archive & archive, item_bone_t &b)
{
	// Nothing to save
	//archive( );
}

template<class Archive>
void serialize(Archive & archive, item_chopping_t &tag)
{
	// Nothing to save
	//archive( );
}

template<class Archive>
void serialize(Archive & archive, item_digging_t &tag)
{
	// Nothing to save
	//archive( );
}

template<class Archive>
void serialize(Archive & archive, item_drink_t &tag)
{
	// Nothing to save
	//archive( );
}

template<class Archive>
void serialize(Archive & archive, item_farming_t &tag)
{
	// Nothing to save
	//archive( );
}

template<class Archive>
void serialize(Archive & archive, item_fertilizer_t &tag)
{
	// Nothing to save
	//archive( );
}

template<class Archive>
void serialize(Archive & archive, item_food_prepared_t &f)
{
	// Nothing to save
	//archive( );
}

template<class Archive>
void serialize(Archive & archive, item_food_t &f)
{
	// Nothing to save
	//archive( );
}

template<class Archive>
void serialize(Archive & archive, item_hide_t &tag)
{
	// Nothing to save
	//archive( );
}

template<class Archive>
void serialize(Archive & archive, item_leather_t &tag)
{
	// Nothing to save
	//archive( );
}

template<class Archive>
void serialize(Archive & archive, item_melee_t &tag)
{
	// Nothing to save
	//archive( );
}

template<class Archive>
void serialize(Archive & archive, item_ranged_t &tag)
{
	// Nothing to save
	//archive( );
}

template<class Archive>
void serialize(Archive & archive, item_seed_t &s)
{
	// Nothing to save
	archive(s.grows_into);
}


template<class Archive>
void serialize(Archive & archive, item_skull_t &tag)
{
	// Nothing to save
	//archive( );
}


template<class Archive>
void serialize(Archive & archive, item_spice_t &tag)
{
	// Nothing to save
	//archive( );
}


template<class Archive>
void serialize(Archive & archive, item_topsoil_t &tag)
{
	// Nothing to save
	//archive( grows_into );
}

template<class Archive>
void serialize(Archive & archive, item_t &i)
{
	archive(i.item_name, i.item_tag, i.type, i.material, i.stack_size, i.clothing_glyph, i.clothing_color, i.clothing_layer); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, item_carried_t &i)
{
	archive(i.location, i.carried_by); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, item_creator_t &i)
{
	archive(i.creator_id, i.creator_name);
}

template<class Archive>
void serialize(Archive & archive, item_quality_t &i)
{
	// Nothing to save
	archive(i.quality);
}

template<class Archive>
void serialize(Archive & archive, item_stored_t &s)
{
	archive(s.stored_in); // serialize things by passing them to the archive
}


template<class Archive>
void serialize(Archive & archive, item_wear_t &w)
{
	archive(w.wear);
}

template<class Archive>
void serialize(Archive & archive, designated_miner_t &m)
{
	// Nothing to save
	//archive( );
}

template<class Archive>
void serialize(Archive & archive, ai_tag_work_stockpiles_t &m)
{
	// Nothing to save
	archive( m.tool_id, m.step, m.destination );
}

template<class Archive>
void serialize(Archive & archive, flying_t &m)
{
	// Nothing to save
	//archive( );
}

template<class Archive>
void serialize(Archive & archive, ai_tag_work_deconstruct &m)
{
	// Nothing to save
	archive(m.step, m.target_building);
}

namespace bengine {
	namespace impl
	{
		my_ecs_t ecs;
	}

	void ecs_save(std::unique_ptr<std::ofstream> &lbfile) noexcept
	{
		cereal::PortableBinaryOutputArchive oarchive(*lbfile);
		oarchive(impl::ecs);
	}

	void ecs_load(std::unique_ptr<std::ifstream> &lbfile) noexcept
	{
		// TODO: Clear everything!
		cereal::PortableBinaryInputArchive iarchive(*lbfile);
		iarchive(impl::ecs);
		for (auto &e : impl::ecs.entities)
		{
			e.second->ecs = &bengine::impl::ecs;
		}
	}
}