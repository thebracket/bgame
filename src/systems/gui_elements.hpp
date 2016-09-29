#pragma once

#include <sstream>
#include <rltk.hpp>
#include <functional>
#include "mouse_input_system.hpp"

struct base_gui {
    virtual void render()=0;
};

struct map_static_text : public base_gui {
    map_static_text(const int &X, const int &Y, const std::string &str, const rltk::color_t FG = rltk::colors::WHITE, const rltk::color_t BG = rltk::colors::BLACK) 
        : x(X), y(Y), text(str), fg(FG), bg(BG) {}

    int x,y;
    std::string text;
    rltk::color_t fg;
    rltk::color_t bg;
    virtual void render() override final {
        rltk::term(1)->print(x,y,text,fg,bg);
    }
};

struct gui_static_text : public base_gui {
    gui_static_text(const int &X, const int &Y, const std::string &str, const rltk::color_t FG = rltk::colors::WHITE, const rltk::color_t BG = rltk::colors::BLACK) 
        : x(X), y(Y), text(str), fg(FG), bg(BG) {}

    int x,y;
    std::string text;
    rltk::color_t fg;
    rltk::color_t bg;
    virtual void render() override final {
        rltk::term(3)->print(x,y,text,fg,bg);
    }
};

struct gui_power_meter : public base_gui {
    int current_power;
    int total_capacity;
    rltk::color_t alert_color;

    gui_power_meter(const int &current, const int &total, const rltk::color_t &col) : current_power(current), total_capacity(total), alert_color(col) {}

    virtual void render() override final {
        float power_pct = (float)current_power / (float) total_capacity;
        if (power_pct < 0.5) {
            alert_color = lerp(rltk::colors::RED, rltk::colors::ORANGE, power_pct*2.0F);
        } else {
            alert_color = lerp(rltk::colors::ORANGE, rltk::colors::WHITE, (power_pct-0.5F)*2.0F);
        }
        std::stringstream power_ss;
        power_ss << " Power: " << current_power << "/" << total_capacity << " ";

        const int power_width = rltk::term(2)->term_width - 30;
        const int power_pips_filled = (float)power_width * power_pct;
        const float pct_per_pip = 1.0 / (float)total_capacity;
        for (int x=0; x<power_width; ++x) {
            const rltk::color_t pip_color = lerp(rltk::colors::GREY, alert_color, pct_per_pip * (float)x); 
            if (x <= power_pips_filled) {
                rltk::term(1)->set_char(x+15, 0, rltk::vchar{176, pip_color, rltk::colors::BLACK});
            } else {
                rltk::term(1)->set_char(x+15, 0, rltk::vchar{7, pip_color, rltk::colors::BLACK});			
            }
        }
        const std::string power_str = power_ss.str();
        const int power_x = ((rltk::term(1)->term_width-30)/2) - (power_str.size() / 2);
        rltk::term(1)->print( power_x, 0, power_str, alert_color );	
    }
};


struct gui_menu_bar : public base_gui {
    gui_menu_bar(const std::vector<std::string> &entries, const int &X, const int &Y, const std::function<void(int &)> &click) 
        : items(entries), x(X), y(Y), on_click(click) {}

    std::vector<std::string> items;
    int x,y;
    std::function<void(int &)> on_click;

    virtual void render() override final {
        int current_x = x;
        int i=0;
        for (const auto &s : items) {
            if (mouse::term1y==y && mouse::term1x >= current_x && mouse::term1x <= current_x + s.size()) {
                rltk::term(1)->print(current_x, y, s, rltk::colors::WHITE, rltk::colors::DARK_GREEN);
                if (mouse::clicked) on_click(i);
            } else {
                rltk::term(1)->set_char(current_x, y, rltk::vchar{s.at(0), rltk::colors::YELLOW, rltk::colors::BLACK});
                rltk::term(1)->print(current_x + 1, y, s.substr(1));
            }
            current_x += s.size()+1;
            ++i;
        }
    }
};
