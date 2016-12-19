#pragma once

#include <rltk.hpp>
#include <boost/container/flat_map.hpp>
#include <vector>

struct screen_render_t {
    screen_render_t(const float &X, const float &Y, const rltk::vchar &C) : x(X), y(Y), c(C) {}
    float x;
    float y;
    rltk::vchar c;
};

extern boost::container::flat_map<int, std::vector<screen_render_t>> renderables;
extern boost::container::flat_map<int, std::vector<std::vector<screen_render_t>>> composite_renderables;

class renderables_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
	bool renderables_changed = true;
};