#pragma once

enum render_mode_t { RENDER_SETTLER, RENDER_SENTIENT };

struct renderable_composite_t {
    render_mode_t render_mode = RENDER_SETTLER;
    uint16_t ascii_char = '@';

	renderable_composite_t() = default;
    explicit renderable_composite_t(const render_mode_t &rm) noexcept : render_mode(rm) {}
    renderable_composite_t(const render_mode_t &rm, const uint16_t &ascii) noexcept : render_mode(rm), ascii_char(ascii) {}
};
