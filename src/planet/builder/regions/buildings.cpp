#include "buildings.hpp"
#include "../../../components/building.hpp"
#include "../../../components/viewshed.hpp"
#include "../../../components/lightsource.hpp"
#include "../../../components/construct_power.hpp"
#include "../../../components/construct_provides_sleep.hpp"
#include "../../../components/construct_provides_door.hpp"
#include "../../../components/smoke_emitter.hpp"
#include "../../../components/calendar.hpp"
#include "../../../components/world_position.hpp"
#include "../../../components/logger.hpp"
#include "../../../components/designations.hpp"
#include "../../../raws/raws.hpp"
#include "../../../raws/materials.hpp"
#include "../../../raws/buildings.hpp"
#include "../../../components/receives_signal.hpp"
#include "../../../components/camera_options.hpp"

using namespace region;

void add_building(std::string tag, const int x, const int y, const int z, const std::size_t &civ_owner) {
    auto building = building_defs.find(tag);
    if (building == building_defs.end()) std::cout << "Warning: do not know how to build " << tag << "\n";

    auto new_building = create_entity()
        ->assign(position_t{x, y, z})
        ->assign(building_t{ tag, building->second.width, building->second.height, building->second.glyphs,
                             building->second.glyphs_ascii, true, civ_owner, 10, 10 });

    for (const building_provides_t &provides : building->second.provides) {
        if (provides.provides == provides_sleep) new_building->assign(construct_provides_sleep_t{});
    }

    if (tag == "storage_locker") {
        spawn_item_in_container(new_building->id, "personal_survival_shelter_kit", get_material_by_tag("plasteel"));
		spawn_item_in_container(new_building->id, "personal_survival_shelter_kit", get_material_by_tag("plasteel"));
		spawn_item_in_container(new_building->id, "personal_survival_shelter_kit", get_material_by_tag("plasteel"));
		spawn_item_in_container(new_building->id, "camp_fire_kit", get_material_by_tag("plasteel"));
		spawn_item_in_container(new_building->id, "fire_axe", get_material_by_tag("plasteel"));
		spawn_item_in_container(new_building->id, "pickaxe", get_material_by_tag("plasteel"));
        new_building->assign(lightsource_t{16, rltk::colors::WHITE, true});
    } else if (tag == "cordex") {
        new_building->assign(viewshed_t{16, false})
            ->assign(lightsource_t{16, rltk::colors::WHITE, true});
    } else if (tag == "battery") {
        new_building->assign(construct_power_t{20,0,0});
    } else if (tag == "rtg") {
        new_building->assign(construct_power_t{0,1,0})->assign(lightsource_t{16, rltk::colors::WHITE, true});
    } else if (tag == "solar_panel") {
        new_building->assign(construct_power_t{00,0,1});
    } else if (tag == "camp_fire") {
        new_building->assign(lightsource_t{5, rltk::colors::YELLOW});
        new_building->assign(smoke_emitter_t{});
    } else if (tag == "energy_door" || tag == "door") {
        new_building->assign(construct_door_t{})->assign(receives_signal_t{});
    }
}

void add_construction(const int x, const int y, const int z, const std::string type, bool solid, const std::size_t &civ_owner) {

    const auto idx = mapidx(x,y,z);
    const auto plasteel = get_material_by_tag("plasteel");
    const auto wood = get_material_by_tag("wood");

    set_tile(idx, tile_type::FLOOR, false, false, plasteel, 0, true, true);

    if (type == "ship_wall") {
        set_tile(idx, tile_type::WALL, true, true, plasteel, 0, true, true);
    } else if (type == "ship_window") {
        set_tile(idx, tile_type::WINDOW, true, false, plasteel, 0, true, true);
    } else if (type == "ship_floor") {
        set_tile(idx, tile_type::FLOOR, false, false, plasteel, 0, true, true);
    } else if (type == "hut_wall") {
        set_tile(idx, tile_type::WALL, true, true, wood, 0, true, true);
    } else if (type == "hut_floor") {
        set_tile(idx, tile_type::FLOOR, false, false, wood, 0, true, true);
    } else if (type == "ship_up") {
        set_tile(idx, tile_type::STAIRS_UP, false, false, plasteel, 0, true, true);
    } else if (type == "ship_down") {
        set_tile(idx, tile_type::STAIRS_DOWN, false, false, plasteel, 0, true, true);
    } else if (type == "ship_updown") {
        set_tile(idx, tile_type::STAIRS_UPDOWN, false, false, plasteel, 0, true, true);
    } else if (type == "hut_upstairs") {
        set_tile(idx, tile_type::STAIRS_UP, false, false, wood, 0, true, true);
    } else if (type == "hut_downstairs") {
        set_tile(idx, tile_type::STAIRS_DOWN, false, false, wood, 0, true, true);
    } else if (type == "hut_updownstairs") {
        set_tile(idx, tile_type::STAIRS_UPDOWN, false, false, wood, 0, true, true);
    } else if (type == "cordex") {
        add_building("cordex", x, y, z, civ_owner);
    } else if (type == "solar_panel") {
        add_building("solar_panel", x, y, z, civ_owner);
    } else if (type == "cryo_bed") {
        add_building("cryo_bed", x, y, z, civ_owner);
    } else if (type == "storage_locker") {
        add_building("storage_locker", x, y, z, civ_owner);
    } else if (type == "battery") {
        add_building("battery", x, y, z, civ_owner);
    } else if (type == "rtg") {
        add_building("rtg", x, y, z, civ_owner);
    } else if (type == "small_replicator") {
        add_building("small_replicator", x, y, z, civ_owner);
    } else if (type == "campfire") {
        add_building("camp_fire", x, y, z, civ_owner);
    } else if (type == "ship_door") {
        add_building("energy_door", x, y, z, civ_owner);
    } else if (type == "door") {
        //std::cout << "Door owner: " << civ_owner << "\n";
        add_building("door", x, y, z, civ_owner);
    } else {
        std::cout << "Don't know how to build a " << type << "\n";
    }
}

void build_escape_pod(const int crash_x, const int crash_y, const int crash_z) {
    xp::rex_sprite ship("rex/spaceship.xp");
    int z=-2+crash_z;
    for (int layer = 0; layer<5; ++layer) {
        for (int Y=0; Y<ship.get_height(); ++Y) {
            for (int X=0; X<ship.get_width(); ++X) {
                const int x = X-5+crash_x;
                const int y = Y-11+crash_y;

                const vchar * output = ship.get_tile(layer,X,Y);
                if (output != nullptr && output->glyph != 32) reveal(mapidx(x,y, z));
				if (output != nullptr && !xp::is_transparent(output)) {
                    if (output->glyph == 219) {
                        add_construction(x, y, z, "ship_wall", true, 0);
                    } else if (output->glyph == 177) {
                        add_construction(x, y, z, "ship_window", true, 0);
                    } else if (output->glyph == 176) {
                        add_construction(x, y, z, "ship_floor", false, 0);
                    } else if (output->glyph == 'X') {
                        add_construction(x, y, z, "ship_updown", false, 0);
                    } else if (output->glyph == '<') {
                        add_construction(x, y, z, "ship_up", false, 0);
                    } else if (output->glyph == '>') {
                        add_construction(x, y, z, "ship_down", false, 0);
                    } else if (output->glyph == 178) {
                        add_construction(x, y, z, "solar_panel", false, 0);
                    } else if (output->glyph == 241) {
                        add_construction(x, y, z, "battery", false, 0);
                    } else if (output->glyph == 'X') {
                        add_construction(x, y, z, "ship_updown", false, 0);
                    } else if (output->glyph == '0') {
                        add_construction(x, y, z, "cryo_bed", false, 0);
                    } else if (output->glyph == 'X') {
                        add_construction(x, y, z, "ship_updown", false, 0);
                    } else if (output->glyph == 236) {
                        add_construction(x, y, z, "storage_locker", false, 0);
                    } else if (output->glyph == 'C') {
                        add_construction(x, y, z, "cordex", false, 0);
                    } else if (output->glyph == 251) {
                        add_construction(x, y, z, "small_replicator", false, 0);
                    } else if (output->glyph == 232) {
                        add_construction(x, y, z, "rtg", false, 0);
                    } else if (output->glyph == 197) {
                        add_construction(x, y, z, "ship_door", false, 0);
                    } else {
                        if (output->glyph != 32)
                            std::cout << "Warning: No handler for " << (char)output->glyph << " (" << +output->glyph << ")\n";
                    }
                }
            }
        }
        ++z;
    }
}

void build_game_components(const int crash_x, const int crash_y, const int crash_z) {
    calendar_t calendar;
	calendar.defined_shifts.push_back(shift_t{"Early Shift", {
		WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT }
	});
	calendar.defined_shifts.push_back(shift_t{"Day Shift", {
		SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT,	WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT }
	});
	calendar.defined_shifts.push_back(shift_t{"Late Shift", {
		LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT,	WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT }
	});
    
    create_entity()
		->assign(world_position_t{region_x(), region_y(), crash_x, crash_y, crash_z+1})
		->assign(std::move(calendar))
		->assign(designations_t{})
        ->assign(logger_t{})
        ->assign(camera_options_t{});
}

int build_building(xp::rex_sprite &sprite, const int x, const int y, const int z, std::vector<std::tuple<int,int,int>> &spawn_points, const bool active, const std::size_t &civ_id) {
    int n_spawn = 0;
    for (int layer = 0; layer < sprite.get_num_layers(); ++layer) {
        for (int Y=0; Y < sprite.get_height(); ++Y) {
            for (int X = 0; X < sprite.get_width(); ++X) {
                const vchar * output = sprite.get_tile(layer,X,Y);
                int material = 0;
                if (output->foreground.r == 102 && output->foreground.g == 82 && output->foreground.b == 51) {
                    material = 1; // Wood
                } else {
                    //if (output->foreground.r != 0 && output->foreground.g != 0 && output->foreground.b != 0)
                    //    std::cout << +output->foreground.r << "," << +output->foreground.g << "," << +output->foreground.b << "\n";
                }

                if (output->glyph == 219 || output->glyph == 177) {
                    add_construction(x+X, y+Y, z+layer, "hut_wall", true, civ_id);
                } else if (output->glyph == 176) {
                    add_construction(x+X, y+Y, z+layer, "hut_floor", false, civ_id);
                } else if (output->glyph == 197) {
                    //std::cout << "Templater passing civ_id to door: " << civ_id << "\n";
                    add_construction(x+X, y+Y, z+layer, "door", false, civ_id);
                } else if (output->glyph == '<') {
                    add_construction(x+X, y+Y, z+layer, "hut_upstairs", false, civ_id);
                } else if (output->glyph == 'X') {
                    add_construction(x+X, y+Y, z+layer, "hut_updownstairs", false, civ_id);
                } else if (output->glyph == '>') {
                    add_construction(x+X, y+Y, z+layer, "hut_downstairs", false, civ_id);
                } else if (output->glyph == 's') {
                    add_construction(x+X, y+Y, z+layer, "hut_floor", false, civ_id);
                    spawn_points.push_back(std::make_tuple(x+X, y+Y, z+layer));
                    ++n_spawn;
                } else if (output->glyph == 234) {
                    add_construction(x+X, y+Y, z+layer, "hut_floor", false, civ_id);
                    if (active) {
                        add_construction(x+X, y+Y, z+layer, "campfire", false, civ_id);
                    }
                }
            }
        }
    }
    return n_spawn;
}

xp::rex_sprite get_building_template(const std::size_t civ_id, planet_t &planet, rltk::random_number_generator &rng) {
    std::vector<std::string> available_buildings;

    available_buildings.push_back("rex/mud-hut.xp");
    available_buildings.push_back("rex/hovel-wood.xp");
    available_buildings.push_back("rex/longhall-wood.xp");
    available_buildings.push_back("rex/henge-wood.xp");

    const int roll = rng.roll_dice(1, available_buildings.size())-1;
    const std::string building_template = available_buildings[roll];
    std::cout << "Building: " << building_template << "\n";
    return xp::rex_sprite(building_template);
}

void build_buildings(rltk::random_number_generator &rng, const int n_buildings, const bool active,
    std::vector<std::tuple<int,int,int>> &spawn_points, const std::size_t &civ_id, planet_t &planet) 
{
    std::cout << "Civ ID#: " << civ_id << "\n";
    int i=0;
    while (i<n_buildings) {
        auto hut = get_building_template(civ_id, planet, rng);
        xp::rex_sprite * building = &hut;
        bool ok = false;
        int tries = 0;
        int x,y,z;
        while (!ok) {
            ok = true;
            x = rng.roll_dice(1, REGION_WIDTH - 20) + 10;
            y = rng.roll_dice(1, REGION_HEIGHT - 20) + 10;
            z = ground_z(x, y);

            // Find lowest point
            for (int Y=0; Y < building->get_height(); ++Y) {
                for (int X = 0; X < building->get_width(); ++X) {
                    const int tmp_z = ground_z(x+X, y+Y);
                    if (z > tmp_z) z = tmp_z;
                }
            }

            // Check for solid or underwater
            for (int Y=0; Y < building->get_height(); ++Y) {
                for (int X = 0; X < building->get_width(); ++X) {
                    for (int Z = 0; Z < building->get_num_layers(); ++Z) {
                        const auto idx = mapidx(X+x, Y+y, Z+z);
                        if (solid(idx)) ok = false;
                        if (water_level(idx) > 0) ok = false;
                        if (region::tile_type(idx) == tile_type::SOLID) ok = false;
                    }
                }
            }

            // Not too close to the escape pod!
            if (distance2d(x,y, REGION_WIDTH/2, REGION_HEIGHT/2) < 15.0F) ok = false;

            if (!ok) ++tries;
            if (tries > 50) ok = true;
        }

        // Spawn the hut
        if (tries < 51) {
            std::cout << "Building passing civ_id: " << civ_id << "\n";
            const int n_spawn = build_building(*building, x, y, z, spawn_points, active, civ_id);
            i += (n_spawn -1);
        }
    }
}
