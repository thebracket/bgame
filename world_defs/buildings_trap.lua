-- Trap buildings

buildings["cage_trap"] = {
    name = "Cage Trap",
    components = { { item="cage", qty=1}, { item="mechanism", qty=1 } },
    skill = { name="Construction", difficulty=10 },
    provides = { cage_trap={energy_cost=0} },
    render = {
        width=1, height=1, tiles={
            {glyph=glyphs['cage'], foreground = colors['green'], background = colors['black']}
        }
    },
    render_ascii = {
        width=1, height=1, tiles={
            {glyph=glyphs['cage_ascii'], foreground = colors['green'], background = colors['black']}
        }
    }
};

buildings["stonefall_trap"] = {
    name = "Stonefall Trap",
    components = { { item="stone_boulder", qty=1}, { item="mechanism", qty=1 } },
    skill = { name="Construction", difficulty=10 },
    provides = { stonefall_trap={energy_cost=0} },
    render = {
        width=1, height=1, tiles={
            {glyph=glyphs['small_circle'], foreground = colors['red'], background = colors['black']}
        }
    },
    render_ascii = {
        width=1, height=1, tiles={
            {glyph=glyphs['small_circle'], foreground = colors['red'], background = colors['black']}
        }
    }
};

buildings["weapon_trap"] = {
    name = "Blade Trap",
    components = { { item="trap_blade", qty=1}, { item="mechanism", qty=1 } },
    skill = { name="Construction", difficulty=10 },
    provides = { blade_trap={energy_cost=0} },
    render = {
        width=1, height=1, tiles={
            {glyph=glyphs['clubs'], foreground = colors['red'], background = colors['black']}
        }
    },
    render_ascii = {
        width=1, height=1, tiles={
            {glyph=glyphs['clubs'], foreground = colors['red'], background = colors['black']}
        }
    }
};

buildings["spike_trap"] = {
    name = "Retractable Spikes",
    components = { { item="retractable_spike", qty=1}, { item="mechanism", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    provides = { spike_trap={energy_cost=0} },
    render = {
        width=1, height=1, tiles={
            {glyph=glyphs['spikes_retracted'], foreground = colors['red'], background = colors['black']}
        }
    },
    render_ascii = {
        width=1, height=1, tiles={
            {glyph=glyphs['o'], foreground = colors['red'], background = colors['black']}
        }
    }
};

buildings["lever"] = {
    name = "Lever",
    components = { { item="mechanism", qty=1 } },
    skill = { name="Construction", difficulty=12 },
    provides = { lever={energy_cost=0} },
    render = {
        width=1, height=1, tiles={
            {glyph=glyphs['lever_off'], foreground = colors['red'], background = colors['black']}
        }
    },
    render_ascii = {
        width=1, height=1, tiles={
            {glyph=glyphs['marker'], foreground = colors['red'], background = colors['black']}
        }
    }
};
