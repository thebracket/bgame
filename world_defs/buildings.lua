buildings = {
	cordex = {
		name = "Cordex",
		description = "This is you, the ship-borne AI trying to keep the motley crew of meat-bag morons from your ship alive.",
		components = { { item="cordex_core", qty=1} },
		skill = { name="Construction", difficulty=25 },
		render_rex = "cordex.xp",
        vox=voxelId("cordex"),
        blocked="-X-XXX-X-"
	},
	lamp = {
		name = "Lamp",
		description = "A simple lamp, designed to provide light.",
		components = { { item="lamp_kit", qty=1} },
		skill = { name="Construction", difficulty=25 },
		render_rex = "lamp.xp",
		provides = { light={radius=5, color = colors['white']} },
        vox=voxelId("floorlight")
	},
	ship_defense_turret = {
		name = "Ship Defense Turret",
		description = "An automated ship defense turret, designed to deter critters and blackguards from entering the ship without permission. It has an angry personality.",
	    components = { { item="ship_turret_kit", qty=1 } },
		skill = { name="Construction", difficulty=25 },
		render_rex = "defense_turret.xp",
		vox=voxelId("turret")
	},
	wall = {
		name = "Wall", components = { { item="block", qty=1 } }, skill = { name="Construction", difficulty=10 }, structure=true,
		description = "A solid wall.",
		provides = { wall={energy_cost=0} },
		render_rex = "wall.xp",
		vox = voxelId("wall")
	},
	floor = {
		name = "Floor", components = { { item="block", qty=1 } }, skill = { name="Construction", difficulty=10 }, structure=true,
		description = "A solid floor.",
		provides = { floor={energy_cost=0} },
		render_rex = "floor.xp",
		vox = voxelId("floor")
	},
	stairs_up = {
		name = "Up Ladder", components = { { item="block", qty=1 } }, skill = { name="Construction", difficulty=10 },
		description = "A ladder going up",
		provides = { stairs_up={energy_cost=0} },
		render_rex = "up_stairs.xp",
		vox=voxelId("stairs_up")		
	},
	stairs_down = {
		name = "Down Ladder", components = { { item="block", qty=1 } }, skill = { name="Construction", difficulty=10 },
		description = "A ladder going down",
		provides = { stairs_down={energy_cost=0} },
		render_rex = "down_stairs.xp",
		vox=voxelId("stairs_down")
	},
	stairs_updown = {
		name = "Up/Down Ladder", components = { { item="block", qty=1 } }, skill = { name="Construction", difficulty=10 },
		description = "A ladder going both up and down.",
		provides = { stairs_updown={energy_cost=0} },
		render_rex = "updown_stairs.xp",
		vox=voxelId("stairs_updown")
	},
	ramp = {
		name = "Ramp", components = { { item="block", qty=1 } }, skill = { name="Construction", difficulty=10 }, structure=true,
		description = "An upwards ramp",
		provides = { ramp={energy_cost=0} },
		render_rex = "ramp.xp",
		vox=voxelId("ramp")
	},
	bridge = {
		name = "Bridge", components = { { item="block", qty=1 } }, skill = { name="Construction", difficulty=10 }, structure=true,
		description = "A bridge that can be linked to an input to retract.",
		provides = { ramp={energy_cost=0} },
		render_rex = "floor.xp",
		vox=voxelId("floor")
	}
}