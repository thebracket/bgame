#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../bengine/ecs_impl.hpp"
#include "../bengine/color_t.hpp"

struct renderable_t {
	uint16_t glyph = 0;
	uint16_t glyph_ascii = 0;
	int vox = 0;
	bengine::color_t foreground{};
	bengine::color_t background{};

	renderable_t() = default;
	renderable_t(const uint16_t ch, const uint16_t cha, const bengine::color_t fg, const bengine::color_t bg, const int vx=0) noexcept : glyph(ch), glyph_ascii(cha), vox(vx), foreground(fg), background(bg) {}

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( glyph, glyph_ascii, foreground, background, vox ); // serialize things by passing them to the archive
	}
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<renderable_t>>)