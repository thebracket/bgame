#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../bengine/ecs_impl.hpp"
#include "../bengine/color_t.hpp"

struct renderable_t {
	uint16_t glyph;
	uint16_t glyph_ascii;
	bengine::color_t foreground;
	bengine::color_t background;

	renderable_t() {}
	renderable_t(const uint16_t ch, const uint16_t cha, const bengine::color_t fg, const bengine::color_t bg) : glyph(ch), glyph_ascii(cha), foreground(fg), background(bg) {}

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( glyph, glyph_ascii, foreground, background ); // serialize things by passing them to the archive
	}
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<renderable_t>>)