#pragma once

#include <rltk.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>

using namespace rltk;

struct renderable_t {
	uint16_t glyph;
	color_t foreground;
	color_t background;

	renderable_t() {}
	renderable_t(const uint16_t ch, const color_t fg, const color_t bg) : glyph(ch), foreground(fg), background(bg) {}

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( glyph, foreground, background ); // serialize things by passing them to the archive
	}
};

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<renderable_t>>)