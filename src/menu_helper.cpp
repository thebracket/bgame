#include "menu_helper.hpp"

using namespace rltk;

void draw_splash_backdrop(layer_t * l, sf::RenderTexture &window) {
        sf::Texture * bg = get_texture("backdrop");
        bg->setSmooth(true);
        bg->setRepeated(false);
        sf::Sprite backdrop(*bg);
        const float x_scale = l->w / 1024.0f;
        const float y_scale = l->h / 768.0f; 
        backdrop.setScale(sf::Vector2f( x_scale, y_scale ));
        window.draw(backdrop);

        sf::Texture * logo = get_texture("logo");
        logo->setSmooth(true);
        logo->setRepeated(false);
        sf::Sprite logo_sprite(*logo);
        logo_sprite.setScale(sf::Vector2f( x_scale, y_scale ));
        float x_pos = (l->w / 2.0f) - ((825.0f * x_scale)/2.0f);
        logo_sprite.move(sf::Vector2f(x_pos, 0.0f));
        window.draw(logo_sprite);
}

