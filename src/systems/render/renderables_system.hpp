#pragma once

#include <rltk.hpp>
#include <vector>
#include <boost/container/flat_map.hpp>

struct screen_render_t {
    screen_render_t(const float &X, const float &Y, const float &ofX, const float &ofY, const rltk::vchar &C) : x(X), y(Y), offsetX(ofX), offsetY(ofY), c(C) {}
    float x;
    float y;
	float offsetX;
	float offsetY;
    rltk::vchar c;
};

extern boost::container::flat_map<int, std::vector<screen_render_t>> renderables;
extern boost::container::flat_map<int, std::vector<std::vector<screen_render_t>>> composite_renderables;
extern boost::container::flat_map<int, std::vector<std::size_t>> model_renderables;

class renderables_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
	bool renderables_changed = true;
};
