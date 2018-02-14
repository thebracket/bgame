#include "cerealizer.hpp"
#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/unordered_set.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/array.hpp>
#include "../bengine/ecs_impl.hpp"

// More things to serialize; broken into chunks because of VS's stupid heap size limits
#include "calendar.hpp"
#include "camera_options.hpp"
#include "claimed_t.hpp"
#include "corpse_harvestable.hpp"
#include "corpse_settler.hpp"
#include "designated_lumberjack.hpp"
#include "explosion_t.hpp"
#include "falling.hpp"
#include "game_stats.hpp"
#include "grazer_ai.hpp"
#include "health.hpp"
#include "initiative.hpp"
#include "lever.hpp"
#include "lightsource.hpp"
#include "logger.hpp"
#include "natural_attacks_t.hpp"
#include "renderable.hpp"
#include "renderable_composite.hpp"
#include "riding_t.hpp"
#include "sentient_ai.hpp"
#include "settler_ai.hpp"
#include "sleep_clock_t.hpp"
#include "slidemove.hpp"
#include "species.hpp"
#include "stockpile.hpp"
#include "viewshed.hpp"
#include "water_spawner.hpp"
#include "wildlife_group.hpp"
#include "world_position.hpp"
#include "helpers/shift.hpp"
#include "helpers/health_part_t.hpp"
#include "helpers/skill_t.hpp"
#include <iostream>

void dummy_init2() {
	std::cout << "2\n";
}
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<calendar_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<camera_options_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<claimed_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<corpse_harvestable>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<corpse_settler>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<designated_lumberjack_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<explosion_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<falling_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<game_stats_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<grazer_ai>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<health_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<initiative_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<lever_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<lightsource_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<logger_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<name_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<natural_attacks_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<renderable_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<renderable_composite_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<riding_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<sentient_ai>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<settler_ai_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<sleep_clock_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<slidemove_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<species_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<stockpile_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<viewshed_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<water_spawner_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<wildlife_group>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<world_position_t>>)

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
	archive(c.camera_mode, c.ascii_mode, c.zoom_level);
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
	//archive(  ); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, health_part_t &h)
{
	archive(h.part, h.max_hitpoints, h.current_hitpoints, h.size); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, health_t &h)
{
	archive(h.max_hitpoints, h.current_hitpoints, h.unconscious, h.blind, h.immobile, h.slow, h.no_grasp, h.parts); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, initiative_t &i)
{
	archive(i.initiative, i.initiative_modifier); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, lever_t &l)
{
	archive(l.active, l.targets); // serialize things by passing them to the archive
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
	/*archive(s.permitted_work, s.shift_id, s.job_type_major, s.job_type_minor, s.target_x,
		s.target_y, s.target_z, s.target_id, s.current_tool);
	*/
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

CEREAL_REGISTER_DYNAMIC_INIT(cerealizer2)

