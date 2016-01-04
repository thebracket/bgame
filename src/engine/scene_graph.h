#pragma once

#include "sdl2_backend.h"
#include <vector>
#include <memory>
#include <string>

using std::vector;
using std::unique_ptr;

namespace engine {
 
class base_node {
public:
    virtual void render(sdl2_backend * SDL)=0;
};
  
/*
 * Provides an interface to the back-end for rendering, by describing
 * what to render and where.
 */ 
class scene_graph {
public:
    void render(sdl2_backend * sdl) {
	sdl->clear_screen();
	for (const unique_ptr<base_node> &child : children) {
	    child->render(sdl);
	}
	sdl->present();
    }
    
    vector<unique_ptr<base_node>> children;
  
};

class scene_background : public base_node {
public:
    scene_background(const std::string tag) : image_tag(tag) {}
    std::string image_tag;
    
    virtual void render(sdl2_backend * SDL) {
	const SDL_Rect big_rect{0,0,1024,768};
	const auto screen_size = SDL->get_screen_size();
	const SDL_Rect full_screen{0,0, screen_size.first, screen_size.second};
	SDL->render_bitmap(image_tag, big_rect, full_screen);
    }
};

class scene_blit : public base_node {
public:
    scene_blit(const std::string &tag, const SDL_Rect &source, const SDL_Rect &dest) : image_tag(tag),
	src(source), dst(dest) {}
    std::string image_tag;
    SDL_Rect src;
    SDL_Rect dst;
    
    virtual void render(sdl2_backend * SDL) {
	SDL->render_bitmap(image_tag, src, dst);
    }
};

class scene_blit_centered : public base_node {
public:
    scene_blit_centered(const std::string &tag, const int &X, const int &Y) : image_tag(tag),
	x(X), y(Y) {}
    std::string image_tag;
    int x, y;
    
    virtual void render(sdl2_backend * SDL) {
	SDL->render_bitmap_centered(image_tag, x, y);
    }
};

}
