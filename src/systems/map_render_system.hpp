#pragma once

#include <rltk.hpp>

class map_render_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override;
private:
	bool dirty = true;
	void update_clipping_rectangle();
	rltk::vchar get_render_char(const int idx) const;
	rltk::vchar get_render_char_for_base(const uint8_t base_type) const;
};