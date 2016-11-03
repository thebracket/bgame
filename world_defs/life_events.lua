life_events = {
    -- Births
    birth_normal = { 
        min_age=0, 
        max_age=0, 
        description="born as a healthy, normal baby.", 
        weight = 50,
        modifiers = { },  
    },
    birth_sickly = { 
        min_age=0, 
        max_age=0, 
        description="born as a sickly baby.", 
        weight = 5,
        modifiers = { con = -1 },  
    },
    birth_abandoned = { 
        min_age=0, 
        max_age=0, 
        description="born, abandoned at the hospital, and adopted.", 
        weight = 2,
        modifiers = { cha = -1 },  
    },
    birth_basket = { 
        min_age=0, 
        max_age=0, 
        description="born in unknown circumstances, found in a basket outside a church, and adopted.", 
        weight = 1,
        modifiers = { cha = -2 },  
    },

    -- Infancy
    stay_home = { 
        min_age=0, 
        max_age=5, 
        description="remained home with %PRONOUN% parents.", 
        weight = 50,
        modifiers = { },  
    },
    daycare_normal = { 
        min_age=0, 
        max_age=5, 
        description="attended daycare, and had a normal experience with it.", 
        weight = 40,
        modifiers = { cha=1 },  
    },
    daycare_excellent = { 
        min_age=0, 
        max_age=5, 
        description="attended daycare, and had an excellent experience.", 
        weight = 30,
        modifiers = { str=1, dex=1, con=1, int=1, wis=1, cha=1 },  
    },
    daycare_terrible = { 
        min_age=0, 
        max_age=5, 
        description="attended daycare, and experienced terrible neglect.", 
        weight = 5,
        modifiers = { str=-1, dex=-1, con=1, int=-1, wis=-1, cha=-1 },  
    },
    stay_home_terrible = { 
        min_age=0, 
        max_age=5, 
        description="remained home with %PRONOUN% parents, and experienced terrible neglect.", 
        weight = 5,
        modifiers = { str=-1, dex=-1, int=-1, wis=-1, cha=-1 },  
    },
    infant_sickness = { 
        min_age=0, 
        max_age=5, 
        description="became quite unwell.", 
        weight = 1,
        modifiers = { con=-1 },  
    },
    infant_dropped_on_head = { 
        min_age=0, 
        max_age=5, 
        description="was dropped on their head.", 
        weight = 1,
        modifiers = { int=-1,wis=-1 },  
    },
    infant_shaken = { 
        min_age=0, 
        max_age=5, 
        description="was shaken by an angry parent.", 
        weight = 1,
        modifiers = { con=-1 },  
    },

    -- School age
    school_normal = { 
        min_age=6, 
        max_age=16, 
        description="attended school, and was quite unremarkable.", 
        weight = 60,
        modifiers = { },  
    },
    homeschool_normal = { 
        min_age=6, 
        max_age=16, 
        description="was educated at home, and had a quite normal experience.", 
        weight = 30,
        modifiers = { },  
    },
    school_excellent = { 
        min_age=6, 
        max_age=16, 
        description="attended school, and excelled.", 
        weight = 10,
        modifiers = { str=1, dex=1, con=1, int=1, wis=1, cha=1 },  
    },
    school_awful = { 
        min_age=6, 
        max_age=16, 
        description="attended school, and performed terribly.", 
        weight = 10,
        modifiers = { str=-1, dex=-1, con=-1, int=-1, wis=-1, cha=-1 },  
    },
    school_bully = { 
        min_age=6, 
        max_age=16, 
        description="attended school, and neglected academics in the name of hitting people.", 
        weight = 10,
        modifiers = { str=1, dex=1, con=1, int=-1, wis=-1, cha=-1 },  
    },
    school_bullied = { 
        min_age=6, 
        max_age=16, 
        description="attended school, and did well but was horribly bullied by other students.", 
        weight = 10,
        modifiers = { str=-1, dex=-1, con=-1, int=1, wis=1, cha=1 },  
    }
}