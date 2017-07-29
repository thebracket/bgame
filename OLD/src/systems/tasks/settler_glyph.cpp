#include "settler_glyph.hpp"
#include "../../messages/renderables_changed_message.hpp"

namespace tasks {

void change_settler_glyph(rltk::entity_t &e, const rltk::vchar &render_as) {
	/*auto render = e.component<renderable_t>();
	render->foreground = render_as.foreground;
	render->background = render_as.background;
	render->glyph = render_as.glyph;*/
	emit_deferred(renderables_changed_message{});
}

}
