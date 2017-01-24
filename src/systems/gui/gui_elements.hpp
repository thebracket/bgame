#pragma once

#include <sstream>
#include <rltk.hpp>
#include <functional>
#include "../input/mouse_input_system.hpp"
#include "../../utils/string_utils.hpp"

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
    virtual void render() override final;
};

struct gui_static_text : public base_gui {
    gui_static_text(const int &X, const int &Y, const std::string &str, const rltk::color_t FG = rltk::colors::WHITE, const rltk::color_t BG = rltk::colors::BLACK) 
        : x(X), y(Y), text(str), fg(FG), bg(BG) {}

    int x,y;
    std::string text;
    rltk::color_t fg;
    rltk::color_t bg;
    virtual void render() override final;
};

struct gui_radio : public base_gui {
    gui_radio(const int &X, const int &Y, const std::string &TITLE, const std::vector<std::string> &opts, const int &sel, const std::function<void(int)> &click) 
        : x(X), y(Y), title(TITLE), options(opts), selected(sel), on_click(click) {}

    int x,y;
    std::string title;
    std::vector<std::string> options;
    int selected;
    std::function<void(int)> on_click;

    virtual void render() override final;
};

struct gui_power_meter : public base_gui {
    int current_power;
    int total_capacity;
    rltk::color_t alert_color;

    gui_power_meter(const int &current, const int &total, const rltk::color_t &col) : current_power(current), total_capacity(total), alert_color(col) {}

    virtual void render() override final;
};


struct gui_menu_bar : public base_gui {
    gui_menu_bar(const std::vector<std::string> &entries, const int &X, const int &Y, const std::function<void(int &)> &click) 
        : items(entries), x(X), y(Y), on_click(click) {}

    std::vector<std::string> items;
    int x,y;
    std::function<void(int &)> on_click;

    virtual void render() override final;
};

struct gui_tab_set : public base_gui {
    int x, y;
    std::vector<std::pair<std::string, std::function<void()>>> tabs;
    int current_tab;

    gui_tab_set(const int &X, const int &Y, const std::vector<std::pair<std::string, std::function<void()>>> &TABS, const int &active) :
        x(X), y(Y), tabs(TABS), current_tab(active) {}

    virtual void render() override final;
};

struct gui_button : public base_gui {
    gui_button(const int &X, const int &Y, const std::string &t, const std::function<void()> click, bool highlight=false) 
        : x(X), y(Y), title(t), on_click(click), active(highlight) {}

    int x,y;
    std::string title;
    std::function<void()> on_click;
    bool active;

    virtual void render() override final;
};

struct gui_table_col {
    int col_width;
    virtual void render(const int &x, const int &y)=0;
};

struct gui_table_text : public gui_table_col {
    gui_table_text(const int &w, const std::string &s) : text(s) {
        col_width = w;
    }
    gui_table_text(const int &w, const std::string &s, const rltk::color_t &FG, const rltk::color_t &BG) : text(s) {
        col_width = w;
        colors = std::make_pair(FG, BG);
    }

    std::string text;
    std::pair<rltk::color_t, rltk::color_t> colors{rltk::colors::BLACK, rltk::colors::BLACK};

    virtual void render(const int &x, const int &y) override final;
};

struct gui_table_button : public gui_table_col {
    gui_table_button(const int &w, const std::string &s, const std::function<void()> &click) : text(s), on_click(click) {
        col_width = w;
    }

    std::string text;
    std::pair<rltk::color_t, rltk::color_t> colors{rltk::colors::BLACK, rltk::colors::BLACK};
    std::function<void()> on_click;

    virtual void render(const int &x, const int &y) override final;
};


struct gui_table_row {
    std::vector<std::unique_ptr<gui_table_col>> cols;
};

struct gui_table : public base_gui {
    std::vector<gui_table_row> rows;
    int x,y;

    gui_table(const int &X, const int &Y) : x(X), y(Y) {} 

    virtual void render() override final;
};

struct gui_dialog : public base_gui {
    gui_dialog(const std::string &t, const std::function<void()> &close) : title(t), on_close(close) {}

    std::string title;
    std::function<void()> on_close;
    std::vector<std::unique_ptr<base_gui>> children;

    virtual void render() override final;
};

struct gui_popup_menu : public base_gui {
    gui_popup_menu(const int &X, const int &Y, const bool &left, const bool &down, const std::function<void()> cancel) 
        : x(X), y(Y), face_left(left), face_down(down), on_cancel(cancel) {}

    int x,y;
    bool face_left;
    bool face_down;
    std::vector<std::pair<std::string, std::function<void()>>> options;
    std::function<void()> on_cancel;

    virtual void render() override final;
};
