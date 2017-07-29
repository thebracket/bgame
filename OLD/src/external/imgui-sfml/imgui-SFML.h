#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>
#include <imgui/imgui.h>

namespace sf
{
    class Event;
    class RenderTarget;
    class RenderWindow;
    class Sprite;
    class Texture;
    class Window;
}

namespace ImGui
{
namespace SFML
{
    template<typename T>
    inline ImVec2 to_imvec2(const T &v) {
        return ImVec2{ static_cast<float>(v.x), static_cast<float>(v.y) };
    }

    inline sf::Vector2f from_imvec2(const ImVec2 &v) {
        return sf::Vector2f{static_cast<float>(v.x), static_cast<float>(v.y)};
    }

    inline ImVec4 to_imvec4(const sf::Color &v) {
        return ImVec4{ v.r/255.0F, v.g/255.0F, v.b/255.0F, v.a/255.0F };
    }

    void Init(sf::RenderTarget& target, sf::Texture* fontTexture = NULL);

    void ProcessEvent(const sf::Event& event);

    void Update(sf::RenderWindow& window, sf::Time dt);
    void Update(sf::Window& window, sf::RenderTarget& target, sf::Time dt);
    void Update(const sf::Vector2i& mousePos, const sf::Vector2u& displaySize, sf::Time dt);

    void Shutdown();

    void createFontTexture(sf::Texture& texture);
    void setFontTexture(sf::Texture& texture);
}

// custom ImGui widgets for SFML stuff

// Image overloads
    void Image(const sf::Texture& texture,
        const sf::Color& tintColor = sf::Color::White,
        const sf::Color& borderColor = sf::Color::Transparent);
    void Image(const sf::Texture& texture, const sf::Vector2u& size,
        const sf::Color& tintColor = sf::Color::White,
        const sf::Color& borderColor = sf::Color::Transparent);
    void Image(const sf::Texture& texture, const sf::FloatRect& textureRect,
        const sf::Color& tintColor = sf::Color::White,
        const sf::Color& borderColor = sf::Color::Transparent);
    void Image(const sf::Texture& texture, const sf::Vector2u& size, const sf::FloatRect& textureRect,
        const sf::Color& tintColor = sf::Color::White,
        const sf::Color& borderColor = sf::Color::Transparent);

    void Image(const sf::Sprite& sprite,
        const sf::Color& tintColor = sf::Color::White,
        const sf::Color& borderColor = sf::Color::Transparent);
    void Image(const sf::Sprite& sprite, const sf::Vector2u& size,
        const sf::Color& tintColor = sf::Color::White,
        const sf::Color& borderColor = sf::Color::Transparent);

// ImageButton overloads
    bool ImageButton(const sf::Texture& texture, const int framePadding = -1,
                     const sf::Color& bgColor = sf::Color::Transparent,
                     const sf::Color& tintColor = sf::Color::White);
    bool ImageButton(const sf::Texture& texture, const sf::Vector2u& size, const int framePadding = -1,
                     const sf::Color& bgColor = sf::Color::Transparent, const sf::Color& tintColor = sf::Color::White);

    bool ImageButton(const sf::Sprite& sprite, const int framePadding = -1,
                     const sf::Color& bgColor = sf::Color::Transparent,
                     const sf::Color& tintColor = sf::Color::White);
    bool ImageButton(const sf::Sprite& sprite, const sf::Vector2u& size, const int framePadding = -1,
                     const sf::Color& bgColor = sf::Color::Transparent,
                     const sf::Color& tintColor = sf::Color::White);

// Draw_list overloads. All positions are in relative coordinates (relative to top-left of the current window)
    void DrawLine(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Color& col, float thickness = 1.0f);
    void DrawRect(const sf::FloatRect& rect, const sf::Color& color, float rounding = 0.0f, int rounding_corners = 0x0F, float thickness = 1.0f);
    void DrawRectFilled(const sf::FloatRect& rect, const sf::Color& color, float rounding = 0.0f, int rounding_corners = 0x0F);
}
