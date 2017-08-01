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
        description="remained home with their parents.", 
        weight = 50,
        modifiers = { },
        skills = { "Literacy" }
    },
    daycare_normal = { 
        min_age=0, 
        max_age=5, 
        description="attended daycare, and had a normal experience with it.", 
        weight = 40,
        modifiers = { cha=1 },  
        skills = { "Literacy" }
    },
    daycare_excellent = { 
        min_age=0, 
        max_age=5, 
        description="attended daycare, and had an excellent experience.", 
        weight = 30,
        modifiers = { str=1, dex=1, con=1, int=1, wis=1, cha=1 },  
        skills = { "Literacy" }
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
    foster_normal = { 
        min_age=0, 
        max_age=5, 
        description="was placed in foster care, and bounced around the system.", 
        weight = 10,
        modifiers = { wis=-1 },  
        skills = { "Literacy" },
        precludes = { "stay_home", "stay_home_terrible" }
    },
    adopted = { 
        min_age=0, 
        max_age=5, 
        description="was adopted by a foster parent.", 
        weight = 10,
        modifiers = { cha=1 },  
        skills = { "Literacy" },
        requires = { "foster_normal" },
        precludes = { "adopted" }
    },

    -- School age
    school_normal = { 
        min_age=6, 
        max_age=16, 
        description="attended school, and was quite unremarkable.", 
        weight = 60,
        modifiers = { },
        skills = { "Literacy" }
    },
    homeschool_normal = { 
        min_age=6, 
        max_age=16, 
        description="was educated at home, and had a quite normal experience.", 
        weight = 30,
        modifiers = { },
        skills = { "Literacy" }
    },
    school_excellent = { 
        min_age=6, 
        max_age=16, 
        description="attended school, and excelled.", 
        weight = 10,
        modifiers = { str=1, dex=1, con=1, int=1, wis=1, cha=1 }, 
        skills = { "Literacy", "Chemistry" } 
    },
    school_awful = { 
        min_age=6, 
        max_age=16, 
        description="attended school, and performed terribly.", 
        weight = 10,
        modifiers = { str=-1, dex=-1, con=-1, int=-1, wis=-1, cha=-1 },
        skills = { "Stealth" } 
    },
    school_bully = { 
        min_age=6, 
        max_age=16, 
        description="attended school, and neglected academics in the name of hitting people.", 
        weight = 10,
        modifiers = { str=1, dex=1, con=1, int=-1, wis=-1, cha=-1 }, 
        skills = { "Melee Attacks" } 
    },
    school_bullied = { 
        min_age=6, 
        max_age=16, 
        description="attended school, and did well but was horribly bullied by other students.", 
        weight = 10,
        modifiers = { str=-1, dex=-1, con=-1, int=1, wis=1, cha=1 },
        skills = { "Stealth" }
    },
    school_sports_won = { 
        min_age=6, 
        max_age=16, 
        description="won a sporting event at school.", 
        weight = 10,
        modifiers = { str=1, dex=1 },
        precludes = { "school_sports_won" }
    },
    school_sports_injury = { 
        min_age=6, 
        max_age=16, 
        description="was injured in a sporting event at school.", 
        weight = 10,
        modifiers = { str=-1, dex=-1 },
        skills = { "Acrobatics" } 
    },
    school_sports_truant = { 
        min_age=6, 
        max_age=16, 
        description="missed a lot of school, for fun.", 
        weight = 10,
        modifiers = { int=-1 },
        skills = { "Acrobatics", "Negotiation" }
    },
    school_dance = { 
        min_age=6, 
        max_age=16, 
        description="performed amazingly at the school dance.", 
        weight = 10,
        modifiers = { dex=1,cha=1 },
        precludes = { "school_dance" },
        skills = { "Acrobatics" }
    },
    school_poetry = { 
        min_age=6, 
        max_age=16, 
        description="won a poetry contest at school.", 
        weight = 10,
        modifiers = { int=1, cha=1 },
        precludes = { "school_poetry" },
        skills = { "Literacy" }
    },
    school_drugs = { 
        min_age=12, 
        max_age=16, 
        description="had troubles with drugs at school.", 
        weight = 10,
        modifiers = { int=-1 }, 
        skills = { "Chemistry" }
    },
    school_dating = { 
        min_age=12, 
        max_age=16, 
        description="missed a lot of school, due to excessive dating.", 
        weight = 20,
        modifiers = { int=-1, cha=1 },
        skills = { "Negotiation" } 
    },
    school_military = { 
        min_age=8, 
        max_age=16, 
        description="attended the Eden Guard cadet program.", 
        weight = 10,
        modifiers = { str=1,dex=1,wis=-1 },
        skills = { "Ranged Attacks", "Melee Attacks", "Stealth" }
    },
    school_leadership = { 
        min_age=12, 
        max_age=16, 
        description="attended Eden Guard leadership training at school.", 
        weight = 10,
        modifiers = { wis=-1,cha=1,dex=1 },
        requires = { "school_military" },
        skills = { "Ranged Attacks", "Melee Attacks", "Negotiation" }
    },

    -- Young adulthood
    further_education = { 
        min_age=16, 
        max_age=21, 
        description="attended further education.", 
        weight = 20,
        modifiers = { int=1,wis=1 },
        precludes = { "vocational_training" },
        skills = { "Literacy" }
    },
    vocational_training = { 
        min_age=16, 
        max_age=21, 
        description="attended trade school.", 
        weight = 20,
        modifiers = { str=1,dex=1 },  
    },
    eden_guard = { 
        min_age=16, 
        max_age=99, 
        description="served as a grunt in the Eden Guard.", 
        weight = 20,
        modifiers = { str=1,dex=1,con=1 },  
        skills = { "Ranged Attacks", "Melee Attacks", "Stealth" }
    },
    eden_guard_squad_leader = { 
        min_age=16, 
        max_age=99, 
        description="served as a squad leader in the Eden Guard.", 
        weight = 20,
        modifiers = { str=1,dex=1,cha=1,eth=1 },  
        requires = { "eden_guard" },
        skills = { "Ranged Attacks", "Melee Attacks", "Negotiation" }
    },
    ganger = { 
        min_age=16, 
        max_age=99, 
        description="ran with a gang in the Eden Hive.", 
        weight = 5,
        modifiers = { str=1,dex=1,con=1,int=-1,wis=-1,eth=-1 },  
        skills = { "Ranged Attacks", "Melee Attacks" }
    },
    gang_leader = { 
        min_age=16, 
        max_age=99, 
        description="lead a gang in the Eden Hive.", 
        weight = 2,
        modifiers = { str=1,dex=1,cha=2,int=-1,wis=-1,eth=-1 }, 
        requires = { "ganger" },
        skills = { "Ranged Attacks", "Melee Attacks", "Negotiation" }
    },
    prison = { 
        min_age=16, 
        max_age=99, 
        description="went to prison.", 
        weight = 2,
        modifiers = { str=1,dex=1,int=-1 },  
        skills = { "Melee Attacks" }
    },
    cultist = { 
        min_age=16, 
        max_age=99, 
        description="joined a cult.", 
        weight = 2,
        modifiers = { wis=-1,con=1,eth=-1 },  
        skills = { "Melee Attacks" }
    },
    cult_leader = { 
        min_age=16, 
        max_age=99, 
        description="lead a cult.", 
        weight = 1,
        modifiers = { wis=-1,cha=1,eth=-1 },  
        skills = { "Negotiation", "Chemistry" }
    },
    cam_exhibitionist = { 
        min_age=16, 
        max_age=99, 
        description="worked as a trid-cam model.", 
        weight = 2,
        modifiers = { cha=1,com=1,eth=-1 },  
        skills = { "Negotiation" }
    },
    witness_protection = { 
        min_age=16, 
        max_age=99, 
        description="hid out in witness protection, while %PRONOUN% helped prosecute a notorious villain.", 
        weight = 1,
        modifiers = { eth=2 },  
        skills = { "Stealth", "Negotiation" }
    },
    drug_mule = { 
        min_age=16, 
        max_age=99, 
        description="ran drugs across the borders.", 
        weight = 1,
        modifiers = { dex=1,wis=-1,eth=-1 },  
        skills = { "Ranged Attacks", "Stealth" }
    },
    smuggler = { 
        min_age=16, 
        max_age=99, 
        description="smuggled shipments between factions in Eden.", 
        weight = 1,
        modifiers = { dex=1,wis=-1,eth=-1 },
        skills = { "Ranged Attacks", "Stealth" }
    },
    smuggler_saint = { 
        min_age=16, 
        max_age=99, 
        description="ran medicine to prescribed groups, despite the danger.", 
        weight = 1,
        modifiers = { dex=1,wis=-1,eth=2 },  
        skills = { "Medicine" }
    },
    stowaway = { 
        min_age=16, 
        max_age=99, 
        description="stowed away on a freighter, and saw the galaxy from a bilge-hold.", 
        weight = 1,
        modifiers = { wis=1,int=-1 }, 
        skills = { "Stealth" }
    },
    hitchhicker = { 
        min_age=16, 
        max_age=99, 
        description="hitched a ride on a series of ships, before running out of money and coming home.", 
        weight = 5,
        modifiers = { dex=1,wis=-1,eth=-1 }, 
        skills = { "Negotiation" }
    },
    mundane = { 
        min_age=16, 
        max_age=99, 
        description="had a mundane year.", 
        weight = 50,
        modifiers = { },  
    },

    -- Events that happen to everyone in world-gen
    ark_b = { 
        min_age=99, 
        max_age=99, 
        description="was selected for the 'B' ark, and took off from Eden-Prime in cryogenic stasis.", 
        weight = 50,
        modifiers = { },  
    },
    planetfall = { 
        min_age=99, 
        max_age=99, 
        description="crash landed on an unknown world, with only Cordex for guidance.", 
        weight = 50,
        modifiers = { },  
    },
}