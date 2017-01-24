#include "gui_elements.hpp"

void map_static_text::render() {
    rltk::term(3)->print(x,y,text,fg,bg);
}

void gui_static_text::render() {
    rltk::term(3)->print(x,y,text,fg,bg);
}

void gui_radio::render() {
    rltk::term(3)->print(x, y, title, rltk::colors::YELLOW, rltk::colors::DARKEST_GREEN);

    int current_y = y+1;
    int i=0;
    for (const std::string &opt : options) {
        rltk::color_t bg = rltk::colors::DARKEST_GREEN;
        if (mouse::term3x >= x && mouse::term3x <= (x+opt.size()+4) && mouse::term3y == current_y) {
            bg = rltk::colors::GREEN;
        }

        if (i == selected) {
            rltk::term(3)->print(x, current_y, std::string("[*] ")+opt, rltk::colors::WHITE, bg);
        } else {
            rltk::term(3)->print(x, current_y, std::string("[ ] ")+opt, rltk::colors::WHITE, bg);
        }
        if (mouse::term3x >= x && mouse::term3x <= (x+opt.size()+4) && mouse::term3y == current_y && mouse::clicked) {
            on_click(i);
        }
        ++i;
        ++current_y;
    }
}

void gui_power_meter::render() {
    float power_pct = (float)current_power / (float) total_capacity;
    if (power_pct < 0.5) {
        alert_color = lerp(rltk::colors::RED, rltk::colors::ORANGE, power_pct*2.0F);
    } else {
        alert_color = lerp(rltk::colors::ORANGE, rltk::colors::WHITE, (power_pct-0.5F)*2.0F);
    }
    std::stringstream power_ss;
    power_ss << " Power: " << current_power << "/" << total_capacity << " ";

    const int power_width = rltk::term(3)->term_width - 30;
    const int power_pips_filled = static_cast<int>((float)power_width * power_pct);
    const float pct_per_pip = 1.0F / (float)total_capacity;
    for (int x=0; x<power_width; ++x) {
        const rltk::color_t pip_color = lerp(rltk::colors::GREY, alert_color, pct_per_pip * (float)x); 
        if (x <= power_pips_filled) {
            rltk::term(3)->set_char(x+15, 0, rltk::vchar{176, pip_color, rltk::colors::BLACK});
        } else {
            rltk::term(3)->set_char(x+15, 0, rltk::vchar{7, pip_color, rltk::colors::BLACK});
        }
    }
    const std::string power_str = power_ss.str();
    const int power_x = ((rltk::term(3)->term_width-30)/2) - ((int)power_str.size() / 2);
    rltk::term(3)->print( power_x, 0, power_str, alert_color );
}

void gui_menu_bar::render() {
    int current_x = x;
    int i=0;
    for (const auto &s : items) {
        if (mouse::term3y==y && mouse::term3x >= current_x && mouse::term3x <= current_x + s.size()) {
            rltk::term(3)->print(current_x, y, s, rltk::colors::WHITE, rltk::colors::DARK_GREEN);
            if (mouse::clicked) {
                mouse::clicked = false;
                on_click(i);
            }
        } else {
            rltk::term(3)->set_char(current_x, y, rltk::vchar{s.at(0), rltk::colors::YELLOW, rltk::colors::BLACK});
            rltk::term(3)->print(current_x + 1, y, s.substr(1));
        }
        current_x += (int)s.size()+1;
        ++i;
    }
}

void gui_tab_set::render() {
    int current_x = x;
    int i=0;
    for (const auto &tab : tabs) {
        const auto length = tab.first.size();
        rltk::term(3)->box(current_x, y, (int)length+1, 2, rltk::colors::GREEN, rltk::colors::DARKEST_GREEN);
        if (mouse::term3x >= current_x && mouse::term3x <= current_x+length+1 && mouse::term3y >= y && mouse::term3y <= y+3) {
            rltk::term(3)->print(current_x+1, y+1, tab.first, rltk::colors::YELLOW, rltk::colors::GREEN);
            if (mouse::clicked) tab.second();
        } else {
            if (current_tab != i) {
                rltk::term(3)->print(current_x+1, y+1, tab.first, rltk::colors::GREEN, rltk::colors::DARKEST_GREEN);
            } else {
                rltk::term(3)->print(current_x+1, y+1, tab.first, rltk::colors::WHITE, rltk::colors::DARKEST_GREEN);
            }
        }
        ++i;
        current_x += (int)length+2;
    }
}

void gui_button::render() {
    const int length = (int)title.size();
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

void gui_table_text::render(const int &x, const int &y) {
    rltk::color_t fg = rltk::colors::WHITE;
    rltk::color_t bg = rltk::colors::DARKEST_GREEN;
    if (! (colors.first == rltk::colors::BLACK)) {
        fg = colors.first;
        bg = colors.second;
    }
    rltk::term(3)->print(x, y, max_width_str(text, col_width), fg, bg);
}

void gui_table_button::render(const int &x, const int &y) {
    rltk::color_t fg = rltk::colors::GREEN;
    rltk::color_t bg = rltk::colors::DARKEST_GREEN;
    if (! (colors.first == rltk::colors::BLACK)) {
        fg = colors.first;
        bg = colors.second;
    }
    if (mouse::term3x >= x && mouse::term3x <= x+text.size() && mouse::term3y == y) {
        rltk::term(3)->print(x, y, max_width_str(text, col_width), rltk::colors::WHITE, rltk::colors::GREEN);
        if (mouse::clicked) {
            mouse::clicked = false;
            on_click();
        }
    } else {
        rltk::term(3)->print(x, y, max_width_str(text, col_width), fg, bg);
    }
}

void gui_table::render() {
    int current_y = y;
    for (const auto &row : rows) {
        int current_x = x;
        for (const auto &col : row.cols) {
            col->render(current_x, current_y);
            current_x += col->col_width + 1;
        }
        ++current_y;
    }
}

void gui_dialog::render() {
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
    rltk::term(3)->print_center(box_top+1, title, rltk::colors::WHITE, rltk::colors::DARKEST_GREEN);

    // Close button
    if (mouse::term3x >= box_left+1 && mouse::term3x < box_left + 4 && mouse::term3y == box_top) {
        rltk::term(3)->print(box_left+1, box_top, "[X]", rltk::colors::WHITE, rltk::colors::DARKEST_GREEN);
        if (mouse::clicked) {
            mouse::clicked = false;
            on_close();
        }
    } else {
        rltk::term(3)->print(box_left+1, box_top, "[X]", rltk::colors::GREEN, rltk::colors::DARKEST_GREEN);
    }

    // Children
    for (auto &g : children) g->render();
}

void gui_popup_menu::render() {
    int width = 0;
    for (const auto &opt : options) {
        if (width < opt.first.size()) width = (int)opt.first.size(); 
    }

    int box_left = x;
    int box_right = x + width + 3;
    int box_top = y;
    int box_bottom = y + (int)options.size() + 1;

    rltk::term(4)->fill(box_left+1, box_top, box_right+1, box_bottom, 219, rltk::colors::DARKEST_GREEN, rltk::colors::DARKEST_GREEN);
    rltk::term(4)->box(box_left+1, box_top, width+1, (int)options.size()+1, rltk::colors::DARK_GREEN, rltk::colors::DARKEST_GREEN, true);

    int current_y = box_top+1;
    for (const auto &opt : options) {
        if (mouse::term4y == current_y && mouse::term4x > box_left+1 && mouse::term4x < box_right) {
            rltk::term(4)->print(box_left + 2, current_y, opt.first, rltk::colors::WHITE, rltk::colors::DARKEST_GREEN);
            if (mouse::clicked && opt.second) {
                std::function<void()> on_click = opt.second;
                on_click();
            }
        } else {
            rltk::term(4)->print(box_left + 2, current_y, opt.first, rltk::colors::GREEN, rltk::colors::DARKEST_GREEN);
        }
        ++current_y;
    }

    if (mouse::clicked && (mouse::term4x < box_left || mouse::term4x > box_right || mouse::term4y < box_top || mouse::term4y > box_bottom)) {
        on_cancel();
    }
}

