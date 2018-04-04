#include "stdafx.h"
#include "item.hpp"
#include "../../raws/clothing.hpp"
#include "../../raws/defs/clothing_t.hpp"
#include "../../raws/defs/item_def_t.hpp"
#include "../../raws/raws.hpp"
#include "../../raws/materials.hpp"
#include "../../global_assets/rng.hpp"
#include "../../raws/items.hpp"
#include "../../utils/system_log.hpp"

bengine::color_t colname_to_col(const std::string &col)
{
    if (col == "black") return bengine::color_t((uint8_t )50,50,64);
    if (col == "white") return bengine::color_t((uint8_t )250,250,250);
    if (col == "red") return bengine::color_t((uint8_t )250,50,50);
    if (col == "blue") return bengine::color_t((uint8_t )50,250,50);
    if (col == "navy") return bengine::color_t((uint8_t )50,190,50);
    if (col == "green") return bengine::color_t((uint8_t )50,50,250);
    if (col == "grey") return bengine::color_t((uint8_t )128,128,128);
    if (col == "yellow") return bengine::color_t((uint8_t )250,250,50);
    if (col == "pink") return bengine::color_t((uint8_t )250,105,180);
    if (col == "brown") return bengine::color_t((uint8_t )98, 74, 46);
    if (col == "khaki") return bengine::color_t((uint8_t )240, 230, 140);
	glog(log_target::LOADER, log_severity::warning, "Unknown clothing color: {0}", col);
    return bengine::color_t((uint8_t )250,250,250);
}

item_t::item_t(const std::string name) noexcept : item_tag(name), type(CLOTHING) {
    //std::cout << "[" << item_tag << "]\n";
    auto finder = get_item_def(item_tag);
    if (finder != nullptr) {
        item_name = finder->name;
    } else {
        auto finder2 = get_clothing_by_tag(item_tag);
        if (finder2) {
            item_name = finder2->name;
            clothing_glyph = finder2->clothing_glyph;
			clothing_layer = finder2->clothing_layer;
            if (finder2->colors.size() > 0) {
                clothing_color = colname_to_col(finder2->colors[rng.roll_dice(1, finder2->colors.size())-1]);
            }
        } else {
            item_name = "ERROR";
			glog(log_target::LOADER, log_severity::error, "{0} not found", item_tag);
        }
    }
}

item_t::item_t(const std::string tag, const std::string name, const std::size_t mat, int stack, int clothing) noexcept :
        item_name(name), item_tag(tag), type(ITEM), material(mat), stack_size(stack)
{
    item_name = material_name(mat) + std::string(" ") + item_name;
	clothing_layer = clothing;
}