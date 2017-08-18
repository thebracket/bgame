-- This file defines furniture buildings.

buildings["cryo_bed"] = {
    name = "Cryogenic Bed",
    components = { { item="cryo_bed_kit", qty=1} },
    skill = { name="Construction", difficulty=10 },
    provides = { sleep={energy_cost=0} },
    render = {
        width=1, height=1, tiles={
            {glyph=glyphs['cryo_bed'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=1, height=1, tiles={
            {glyph=glyphs['eight'], foreground = colors['white'], background = colors['black']}
        }
    },
    vox=1
};

buildings["storage_locker"] = {
    name = "A storage locker",
    components = { { item="storage_locker", qty=1} },
    skill = { name="Construction", difficulty=10 },
    provides = { storage={energy_cost=0} },
    render = {
        width=1, height=1, tiles={
            {glyph=glyphs['storage_locker'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=1, height=1, tiles={
            {glyph=glyphs['cabinet'], foreground = colors['white'], background = colors['black']}
        }
    }
};

buildings["personal_survival_shelter"] = {
    name = "Tent",
    components = { { item="personal_survival_shelter_kit", qty=1 } },
    skill = { name="Construction", difficulty=5 },
    provides = { sleep={energy_cost=0} },
    render = {
        width=1, height=1, tiles={
            {glyph=glyphs['tent'], foreground = colors['green'], background = colors['black']}
        }
    },
    render_ascii = {
        width=1, height=1, tiles={
            {glyph=glyphs['arch'], foreground = colors['green'], background = colors['black']}
        }
    }
};

buildings["table"] = {
    name = "Table",
    components = { { item="table", qty=1 } },
    skill = { name="Construction", difficulty=5 },
    provides = { table={energy_cost=0} },
    render = {
        width=1, height=1, tiles= {
            {glyph= glyphs['table'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=1, height=1, tiles= {
            {glyph= glyphs['table'], foreground = colors['white'], background = colors['black']}
        }
    }
};

buildings["chair"] = {
    name = "Chair",
    components = { { item="chair", qty=1 } },
    skill = { name="Construction", difficulty=5 },
    provides = { table={energy_cost=0} },
    render = {
        width=1, height=1, tiles= {
            {glyph= glyphs['chair'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=1, height=1, tiles= {
            {glyph= glyphs['chair'], foreground = colors['white'], background = colors['black']}
        }
    }
};

buildings["door"] = {
    name = "Door",
    components = { { item="door", qty=1 } },
    skill = { name="Construction", difficulty=10 },
    provides = { door={energy_cost=0}, signal={energy_cost=0} },
    render = {
        width=1, height=1, tiles= {
            {glyph= glyphs['door_open'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=1, height=1, tiles= {
            {glyph= glyphs['door'], foreground = colors['white'], background = colors['black']}
        }
    }
};

buildings["energy_door"] = {
    name = "Force Door",
    components = { { item="door", qty=1 } },
    skill = { name="Construction", difficulty=10 },
    provides = { door={energy_cost=10} },
    render = {
        width=1, height=1, tiles= {
            {glyph= glyphs['door_open'], foreground = colors['cyan'], background = colors['black']}
        }
    },
    render_ascii = {
        width=1, height=1, tiles= {
            {glyph= glyphs['door'], foreground = colors['cyan'], background = colors['black']}
        }
    }
};

buildings["bed_simple"] = {
    name = "Simple Bed",
    components = { { item="bed_item", qty=1} },
    skill = { name="Construction", difficulty=10 },
    provides = { sleep={energy_cost=0} },
    render = {
        width=1, height=1, tiles={
            {glyph=glyphs['cryo_bed'], foreground = colors['white'], background = colors['black']}
        }
    },
    render_ascii = {
        width=1, height=1, tiles={
            {glyph=glyphs['eight'], foreground = colors['brown'], background = colors['black']}
        }
    }
};
