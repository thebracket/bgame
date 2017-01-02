-- Helpers for building vegetation
grass_lifecycle = { 10, 28, 90, 90, 3};
fast_lifecycle = { 10, 28, 60, 60, 3};
slow_lifecycle = { 28, 40, 90, 90, 0};

function harvest_normal(r) return { 'none', 'none', r, r } end

function veg_g(a, b, c, d) return { glyphs[a], glyphs[b], glyphs[c], glyphs[d] } end

function build_veg(name, cycles, glyphs, harvest, tags) return { name = name, cycles = cycles, glyphs = glyphs, harvest = harvest, tags = tags; } end

vegetation = {
    -- Grassland types
    dummy_grass = build_veg("Grass", grass_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'grass_long'), harvest_normal('hay'), {'spread'}),
    grass = build_veg("Grass", grass_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'grass_long'), harvest_normal('hay'), {'spread'}),
    daisy = build_veg("Daisy", grass_lifecycle, veg_g('tilled', 'grass_sparse', 'flower_yellow', 'flower_yellow'), harvest_normal('none'), {'spread'}),
    heather = build_veg("Heather", grass_lifecycle, veg_g('tilled', 'grass_sparse', 'flower_purple', 'flower_purple'), harvest_normal('none'), {'spread'}),

    -- Paper/cloth providers
    reeds = build_veg("Reeds, Common", grass_lifecycle, veg_g('tilled', 'grass_sparse', 'reeds', 'reeds'), harvest_normal('reed_thread'), {}),
    reeds_giant = build_veg("Reeds, Giant", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'reeds', 'reeds'), harvest_normal('reed_thread'), {}),
    reeds_paper = build_veg("Reeds, Papyrus", grass_lifecycle, veg_g('tilled', 'grass_sparse', 'reeds', 'reeds'), harvest_normal('reed_paper'), {}),

    -- Vegetables
    artichoke = build_veg("Artichoke", slow_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('artichoke_veg'), {'annual'}),
    asparagus = build_veg("Asparagus", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('asparagus_veg'), {''}),
    string_bean = build_veg("String Bean", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('string_bean_veg'), {''}),
    broad_bean = build_veg("Broad Bean", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('broad_bean_veg'), {''}),
    beetroot = build_veg("Beetroot", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('beetroot_veg'), {''}),
    goya = build_veg("Goya", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('goya_veg'), {''}),
    cabbage = build_veg("Cabbage", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('cabbage_veg'), {''}),
    caper = build_veg("Caper", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('caper_veg'), {''}),
    carrot = build_veg("Carrot", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('carrot_veg'), {''}),
    cassava = build_veg("Cassava", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('cassava_veg'), {''}),
    celery = build_veg("Celery", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('celery_veg'), {''}),
    chickpea = build_veg("Chickpea", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('chickpea_veg'), {''}),
    cowpea = build_veg("Cowpea", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('cowpea_veg'), {''}),
    cucumber = build_veg("Cucumber", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('cucumber_veg'), {''}),
    eggplant = build_veg("Eggplant", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('eggplant_veg'), {''}),
    cress = build_veg("Cress", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('cress_veg'), {''}),
    horned_melon = build_veg("Horned Melon", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('melon_veg'), {''}),
    leek = build_veg("Leek", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('leek_veg'), {''}),
    lentil = build_veg("Lentil", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('lentil_veg'), {''}),
    lettuce = build_veg("Lettuce", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('lettuce_veg'), {''}),
    mung_bean = build_veg("Mung Bean", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('mung_bean_veg'), {''}),
    onion = build_veg("Onion", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('onion_veg'), {''}),
    parsnip = build_veg("Parsnip", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('parsnip_veg'), {''}),
    pea = build_veg("Pea", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('pea_veg'), {''}),
    pepper = build_veg("Pepper", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('pepper_veg'), {''}),
    potato = build_veg("Potato", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('potato_veg'), {''}),
    radish = build_veg("Radish", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('radish_veg'), {''}),
    red_bean = build_veg("Red Bean", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('red_bean_veg'), {''}),
    spinach = build_veg("Spinach", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('spinach_veg'), {''}),
    soybean = build_veg("Soybean", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('soybean_veg'), {''}),
    squash = build_veg("Squash", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('squash_veg'), {''}),
    sweet_potato = build_veg("Sweet Potato", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('sweet_potato_veg'), {''}),
    tomato = build_veg("Tomato", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('tomato_veg'), {''}),
    tomatillo = build_veg("Tomatillo", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('tomatillo_veg'), {''}),
    turnip = build_veg("Turnip", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('turnip_veg'), {''}),
    watermelon = build_veg("Watermelon", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('watermelon_veg'), {''}),
    yam = build_veg("Yam", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('yam_veg'), {''}),
    strawberry = build_veg("Strawberry", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('strawberry_fruit'), {''}),

    -- Nuts
    bambara_groundnut = build_veg("Bambara Groundnut", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('bambara_nut'), {''}),
    peanut = build_veg("Peanut", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('peanut_nut'), {''}),

    -- Herbs
    chickory = build_veg("Chickory", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('chickory_herb'), {''}),
    garlic = build_veg("Garlic", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('garlic_herb'), {''}),
    rhubarb = build_veg("Rhubarb", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_small'), harvest_normal('rhubarb_herb'), {''}),
    lavendar = build_veg("Lavendar", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'grass_long', 'flower_purple'), harvest_normal('lavendar_herb'), {''}),
    sage = build_veg("Sage", fast_lifecycle, veg_g('tilled', 'grass_sparse', 'bush', 'bush'), harvest_normal('sage_herb'), {''})
}
