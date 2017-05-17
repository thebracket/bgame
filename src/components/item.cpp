#include "item.hpp"
#include "../raws/clothing.hpp"
#include "../raws/defs/clothing_t.hpp"
#include "../raws/raws.hpp"
#include "../raws/materials.hpp"
#include "../main/game_rng.hpp"

color_t colname_to_col(const std::string &col)
{
    if (col == "black") return color_t(50,50,64);
    if (col == "white") return color_t(250,250,250);
    if (col == "red") return color_t(250,50,50);
    if (col == "blue") return color_t(50,250,50);
    if (col == "navy") return color_t(50,190,50);
    if (col == "green") return color_t(50,50,250);
    if (col == "grey") return color_t(128,128,128);
    if (col == "yellow") return color_t(250,250,50);
    if (col == "pink") return color_t(250,105,180);
    if (col == "brown") return color_t(98, 74, 46);
    if (col == "khaki") return color_t(240, 230, 140);
    std::cout << "WARNING: Unknown clothing color: " << col << "\n";
    return color_t(250,250,250);
}

item_t::item_t(const std::string name) : item_tag(name), type(CLOTHING) {
    //std::cout << "[" << item_tag << "]\n";
    auto finder = item_defs.find(item_tag);
    if (finder != item_defs.end()) {
        item_name = finder->second.name;
    } else {
        auto finder2 = get_clothing_by_tag(item_tag);
        if (finder2) {
            item_name = finder2->name;
            clothing_glyph = finder2->clothing_glyph;
            if (finder2->colors.size() > 0) {
                clothing_color = colname_to_col(finder2->colors[rng.roll_dice(1, finder2->colors.size())-1]);
            }
        } else {
            item_name = "ERROR";
            std::cout << item_tag << " not found!\n";
        }
    }
}

item_t::item_t(const std::string tag, const std::string name, const std::bitset<NUMBER_OF_ITEM_CATEGORIES> cats,
               const std::size_t mat, int stack) :
        item_name(name), item_tag(tag), category(cats), type(ITEM), material(mat), stack_size(stack)
{
    if (!cats.test(ITEM_FOOD) && !cats.test(ITEM_SPICE) &&
        !cats.test(ITEM_DRINK) && !cats.test(ITEM_SKULL) &&
        !cats.test(ITEM_BONE) && !cats.test(ITEM_HIDE) &&
        !cats.test(ITEM_LEATHER))
    {
        item_name = material_name(mat) + std::string(" ") + item_name;
    }
}