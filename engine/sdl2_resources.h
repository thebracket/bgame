#pragma once

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
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
    
    texture_resource(SDL_Texture * new_texture) {
	image = new_texture;
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

struct font_resource {
    font_resource(const std::string &fn, const std::string &tag, const int &size) : filename(fn), name(tag), point_size(size)
    {
	font = TTF_OpenFont( fn.c_str(), size );
	if (font == nullptr) {
	    std::cout << "Error loading " << fn << "\n";
	    throw 101;
	}
	initialized = true;
    }
    
    void clear() {
	if (initialized) TTF_CloseFont( font );
    }
  
    bool initialized = false;
    std::string filename;
    std::string name;
    int point_size;
    TTF_Font * font;
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
    
    int load_font(const std::string &filename, const std::string &tag, const int &size) {
	const int index = fonts.size();
	font_resource fon = font_resource( filename, tag, size );
	fonts.push_back( fon );
	font_index[tag] = index;
	return index;
    }
    
    void clear() {
	texture_index.clear();
	for (texture_resource &t : textures) t.clear();
	textures.clear();
	
	font_index.clear();
	for (font_resource &f : fonts) f.clear();
	fonts.clear();
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
    
    TTF_Font * get_font_by_id(const int &id) { return fonts[id].font; }
    TTF_Font * get_font_by_tag(const std::string &tag) {
	auto finder = font_index.find(tag);
	if (finder != font_index.end()) {
	    return get_font_by_id( finder->second );
	} else {
	    return nullptr;
	}
    }
    
    int render_text_to_texture(SDL_Renderer * renderer, const std::string &font_tag, const std::string text, const std::string &new_tag, SDL_Color color) {
	TTF_Font * font = get_font_by_tag( font_tag );
	if (font == nullptr) {
	  std::cout << "Null font!\n";
	  throw 101;
	}
	SDL_Surface * surf = TTF_RenderText_Blended( font, text.c_str(), color );
	if (surf == nullptr) {
	  std::cout << "Null surface!\n";
	  throw 101;
	}
	SDL_Texture * texture = SDL_CreateTextureFromSurface( renderer, surf );
	SDL_FreeSurface( surf );

	const int index = textures.size();
	textures.push_back( texture_resource( texture ) );
	texture_index[new_tag] = index;
	return index;
    }
  
private:
    std::vector<texture_resource> textures;
    std::unordered_map<std::string,int> texture_index;
    std::vector<font_resource> fonts;
    std::unordered_map<std::string,int> font_index;
};

}