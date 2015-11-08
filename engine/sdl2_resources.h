#pragma once

#include <SDL2/SDL_image.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

namespace engine {

struct texture_resource {
    texture_resource(SDL_Renderer * renderer, const std::string &fn, const std::string &tag) : filename(fn), name(tag) 
    {
	SDL_Surface * tmp_surface = IMG_Load ( fn.c_str() );
	if ( tmp_surface == NULL ) throw 104;
	image = SDL_CreateTextureFromSurface ( renderer, tmp_surface );
	SDL_FreeSurface ( tmp_surface );
	initialized = true;
    }
        
    void clear() {
	if (initialized) SDL_DestroyTexture(image);
    }
  
    bool initialized = false;
    std::string filename;
    std::string name;
    SDL_Texture * image;
};

struct sdl_resource_manager {
    int load_image(SDL_Renderer * renderer, const std::string &filename, const std::string &tag) {
	int image_flags = IMG_INIT_PNG;
	if ( ! ( IMG_Init ( image_flags ) & image_flags ) ) throw 104;
      
	const int index = textures.size();
	texture_resource res = texture_resource( renderer, filename, tag );
	textures.push_back( std::move( res ) );
	texture_index[tag] = index;
	return index;
    }
    
    void clear() {
	texture_index.clear();
	for (texture_resource &t : textures) t.clear();
	textures.clear();
    }
    
    SDL_Texture * get_texture_by_id(const int &id) { return textures[id].image; }
    SDL_Texture * get_texture_by_tag(const std::string &tag) {
	auto finder = texture_index.find(tag);
	if (finder != texture_index.end()) {
	    return get_texture_by_id( finder->second );
	} else {
	    return nullptr;
	}
    }
  
private:
    std::vector<texture_resource> textures;
    std::unordered_map<std::string,int> texture_index;
};

}