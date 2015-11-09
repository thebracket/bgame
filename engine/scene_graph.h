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
	SDL->render_bitmap(image_tag, big_rect, big_rect);
    }
};

}
