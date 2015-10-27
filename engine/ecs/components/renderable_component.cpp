#include "renderable_component.h"

namespace engine {
namespace ecs {

renderable_component::renderable_component() {
    type = renderable;
    multi_tile = false;
}

renderable_component::renderable_component(const unsigned char GLYPH, const engine::vterm::color_t &FOREGROUND, const engine::vterm::color_t &BACKGROUND) :
    glyph(GLYPH), foreground(FOREGROUND), background(BACKGROUND)
{
    type = renderable;
    multi_tile = false;
}

renderable_component::renderable_component(const engine::vterm::color_t &FOREGROUND, const engine::vterm::color_t &BACKGROUND,
        const char &nwidth, const char &nheight, const vector<unsigned char> &nglyphs
                                          ) :
    foreground(FOREGROUND), background(BACKGROUND), width(nwidth), height(nheight)
{
    type = renderable;
    multi_tile = true;
    for (const unsigned char &c : nglyphs) glyphs.push_back(c);
}

void renderable_component::save(fstream &lbfile) {
    base_component::save(lbfile);
    lbfile.write ( reinterpret_cast<const char *> ( &glyph ), sizeof ( glyph ) );
    unsigned char fg_red = std::get<0>(foreground);
    unsigned char fg_green = std::get<1>(foreground);
    unsigned char fg_blue = std::get<2>(foreground);
    unsigned char bg_red = std::get<0>(background);
    unsigned char bg_green = std::get<1>(background);
    unsigned char bg_blue = std::get<2>(background);
    lbfile.write ( reinterpret_cast<const char *> ( &fg_red ), sizeof ( fg_red ) );
    lbfile.write ( reinterpret_cast<const char *> ( &fg_green ), sizeof ( fg_green ) );
    lbfile.write ( reinterpret_cast<const char *> ( &fg_blue ), sizeof ( fg_blue ) );
    lbfile.write ( reinterpret_cast<const char *> ( &bg_red ), sizeof ( bg_red ) );
    lbfile.write ( reinterpret_cast<const char *> ( &bg_green ), sizeof ( bg_green ) );
    lbfile.write ( reinterpret_cast<const char *> ( &bg_blue ), sizeof ( bg_blue ) );
    lbfile.write ( reinterpret_cast<const char *> ( &multi_tile ), sizeof ( multi_tile ) );
    lbfile.write ( reinterpret_cast<const char *> ( &width ), sizeof ( width ) );
    lbfile.write ( reinterpret_cast<const char *> ( &height ), sizeof ( height ) );
    int mt_size = glyphs.size();
    lbfile.write ( reinterpret_cast<const char *> ( &mt_size ), sizeof ( mt_size ) );
    for (const unsigned char &g : glyphs) {
        lbfile.write ( reinterpret_cast<const char *> ( &g ), sizeof ( g ) );
    }
}

void renderable_component::load(fstream &lbfile) {
    base_component::load(lbfile);
    lbfile.read ( reinterpret_cast<char *> ( &glyph ), sizeof ( glyph ) );
    unsigned char fg_red = 0;
    unsigned char fg_green = 0;
    unsigned char fg_blue = 0;
    unsigned char bg_red = 0;
    unsigned char bg_green = 0;
    unsigned char bg_blue = 0;
    lbfile.read ( reinterpret_cast<char *> ( &fg_red ), sizeof ( fg_red ) );
    lbfile.read ( reinterpret_cast<char *> ( &fg_green ), sizeof ( fg_green ) );
    lbfile.read ( reinterpret_cast<char *> ( &fg_blue ), sizeof ( fg_blue ) );
    lbfile.read ( reinterpret_cast<char *> ( &bg_red ), sizeof ( bg_red ) );
    lbfile.read ( reinterpret_cast<char *> ( &bg_green ), sizeof ( bg_green ) );
    lbfile.read ( reinterpret_cast<char *> ( &bg_blue ), sizeof ( bg_blue ) );
    foreground = engine::vterm::color_t {fg_red, fg_green, fg_blue};
    background = engine::vterm::color_t {bg_red, bg_green, bg_blue};
    lbfile.read ( reinterpret_cast<char *> ( &multi_tile ), sizeof ( multi_tile ) );
    lbfile.read ( reinterpret_cast<char *> ( &width ), sizeof ( width ) );
    lbfile.read ( reinterpret_cast<char *> ( &height ), sizeof ( height ) );
    int mt_size = 0;
    lbfile.read ( reinterpret_cast<char *> ( &mt_size ), sizeof ( mt_size ) );
    for (int i=0; i<mt_size; ++i) {
        char g;
        lbfile.read ( reinterpret_cast<char *> ( &g ), sizeof ( g ) );
        glyphs.push_back(g);
    }
}

}
}