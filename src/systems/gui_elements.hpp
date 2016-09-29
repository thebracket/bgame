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

struct gui_button : public base_gui {
    gui_button(const int &X, const int &Y, const std::string &t, const std::function<void()> click, bool highlight=false) 
        : x(X), y(Y), title(t), on_click(click), active(highlight) {}

    int x,y;
    std::string title;
    std::function<void()> on_click;
    bool active;

    virtual void render() override final {
        const int length = title.size();
        rltk::term(3)->box(x, y, length+2, 2, rltk::colors::GREEN, rltk::colors::DARKEST_GREEN);
        if (mouse::term3x >= x && mouse::term3x <= x+length+1 && mouse::term3y >= y && mouse::term3y <= y+3) {
            rltk::term(3)->print(x+1, y+1, title, rltk::colors::YELLOW, rltk::colors::GREEN);
            if (mouse::clicked) on_click();
        } else {
            if (!active) {
                rltk::term(3)->print(x+1, y+1, title, rltk::colors::GREEN, rltk::colors::DARKEST_GREEN);
            } else {
                rltk::term(3)->print(x+1, y+1, title, rltk::colors::WHITE, rltk::colors::DARKEST_GREEN);
            }
        }
    }
};

struct gui_dialog : public base_gui {
    gui_dialog(const std::string &t, const std::function<void()> &close) : title(t), on_close(close) {}

    std::string title;
    std::function<void()> on_close;
    std::vector<std::unique_ptr<base_gui>> children;

    virtual void render() override final {
        const int term_w = rltk::term(3)->term_width;
        const int term_h = rltk::term(3)->term_height;

        const int box_left = 3;
        const int box_right = term_w - 3;
        const int box_top = 2;
        const int box_bottom = term_h - 2;

        // Panel and header
        rltk::term(3)->fill(box_left, box_top, box_right, box_bottom, 219, rltk::colors::DARKEST_GREEN);
        rltk::term(3)->box(box_left, box_top, box_right-box_left, box_bottom-box_top-1, rltk::colors::DARK_GREEN, rltk::colors::DARKEST_GREEN, true);
        rltk::term(3)->fill(box_left+1, box_top+1, box_right, box_top+2, 219, rltk::colors::DARK_GREEN, rltk::colors::DARK_GREEN);
        rltk::term(3)->print_center(box_top+1, title, rltk::colors::WHITE, rltk::colors::GREEN);

        // Close button
        if (mouse::term3x >= box_left+1 && mouse::term3x < box_left + 4 && mouse::term3y == box_top) {
            rltk::term(3)->print(box_left+1, box_top, "[X]", rltk::colors::WHITE, rltk::colors::DARKEST_GREEN);
            if (mouse::clicked) on_close();
        } else {
            rltk::term(3)->print(box_left+1, box_top, "[X]", rltk::colors::GREEN, rltk::colors::DARKEST_GREEN);
        }

        // Children
        for (auto &g : children) g->render();
    }
};
