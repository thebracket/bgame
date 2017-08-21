-- This file defines buildings relating to power generation or storage

buildings["solar_panel"] = {
    name = "Solar Panel",
    components = { { item="solar_panel_kit", qty=1} },
    skill = { name="Construction", difficulty=15 },
    provides = { generator={energy_cost=10} },
    render = {
        width=1, height=1, tiles={
            {glyph=glyphs['solar_panel'], foreground = colors['cyan'], background = colors['white']}
        }
    },
    render_ascii = {
        width=1, height=1, tiles={
            {glyph=glyphs['hash'], foreground = colors['cyan'], background = colors['white']}
        }
    }
};

buildings["battery"] = {
    name = "A battery bank",
    components = { { item="battery_kit", qty=1} },
    skill = { name="Construction", difficulty=10 },
    provides = { power_storage={energy_cost=20} },
    render = {
        width=1, height=1, tiles={
            {glyph=glyphs['battery'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=1, height=1, tiles={
            {glyph=glyphs['plus_minus'], foreground = colors['red'], background = colors['black']}
        }
    },
    vox=5
};

buildings["rtg"] = {
    name = "A small nuclear power generator",
    components = { {item="refined_plutonium", qty=1} },
    skill = { name="Construction", difficulty=15 },
    provides = { storage={energy_cost=0} },
    render = {
        width=1, height=1, tiles={
            {glyph=glyphs['rtg'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=1, height=1, tiles={
            {glyph=glyphs['plus_minus'], foreground = colors['green'], background = colors['black']}
        }
    },
    vox=3
};
