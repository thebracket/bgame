#pragma once

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <sstream>

namespace engine {

struct texture_resource {
    texture_resource() {}
  
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
    bool deleted = false;
    std::string filename;
    std::string name;
    SDL_Texture * image;
};

struct font_resource {
    font_resource() {}
  
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
  
    void load_image(SDL_Renderer * renderer, const std::string &filename, const std::string &tag) {
	int image_flags = IMG_INIT_PNG;
	if ( ! ( IMG_Init ( image_flags ) & image_flags ) ) throw 104;
      
	texture_resource res = texture_resource( renderer, filename, tag );
	texture_index[tag] = res;
    }
    
    void load_font(const std::string &filename, const std::string &tag, const int &size) {
	font_resource fon = font_resource( filename, tag, size );
	font_index[tag] = fon;
    }
    
    void clear() {
	for (auto it = texture_index.begin(); it != texture_index.end(); ++it) {
	    it->second.clear();
	}	
	texture_index.clear();
	
	for (auto it = font_index.begin(); it != font_index.end(); ++it) {
	    it->second.clear();
	}	
	font_index.clear();
    }
    
    void mark_and_sweep() {
	for ( auto it = texture_index.begin(); it != texture_index.end() ; ) {
	    if (it->second.deleted) {
		it->second.clear();
		texture_index.erase( it++ );
	    } else {
		++it;
	    }
	}
    }
        
    SDL_Texture * get_texture_by_tag(const std::string &tag) {
	auto finder = texture_index.find(tag);
	if (finder != texture_index.end()) {
	    return finder->second.image;
	} else {
	    return nullptr;
	}
    }
    
    TTF_Font * get_font_by_tag(const std::string &tag) {
	auto finder = font_index.find(tag);
	if (finder != font_index.end()) {
	    return finder->second.font;
	} else {
	    return nullptr;
	}
    }
    
    std::string render_text_to_texture(SDL_Renderer * renderer, const std::string &font_tag, const std::string text, const std::string &new_tag, SDL_Color color) {
	TTF_Font * font = get_font_by_tag( font_tag );
	if (font == nullptr) {
	  std::cout << "Null font!\n";
	  throw 101;
	}
	SDL_Surface * surf = TTF_RenderText_Blended( font, text.c_str(), color );
	if (surf == nullptr) {
	  std::cout << "Null surface when rendering [" << text << "]:" << TTF_GetError() << "\n";
	  throw 101;
	}
	SDL_Texture * texture = SDL_CreateTextureFromSurface( renderer, surf );
	SDL_FreeSurface( surf );

	std::stringstream ss;
	ss << "transient_" << transient_id;
	const std::string text_name = ss.str();
	++transient_id;

	texture_resource res = texture_resource( texture );
	res.deleted = true;
	texture_index [ text_name ] = res;
	return text_name;
    }
    
    std::string create_temporary_texture(SDL_Renderer * renderer, const int &width, const int &height) {
	std::stringstream ss;
	ss << "transient_" << transient_id;
	const std::string text_name = ss.str();
	++transient_id;
      
	SDL_Texture * texture = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height );
	texture_resource t(texture);
	texture_index[text_name] = t;
	return text_name;
    }
    
    void mark_texture_for_deletion( const std::string &target ) {
	auto finder = texture_index.find( target );
	if (finder != texture_index.end() ) finder->second.deleted = true;
    }
    
    pair<int,int> text_size( const std::string &font, const string &text ) {
	int w,h;
	TTF_Font * sdl_font = get_font_by_tag( font );
	TTF_SizeText( sdl_font, text.c_str(), &w, &h );
	return std::make_pair(w,h);
    }
  
private:
    std::unordered_map<std::string, texture_resource> texture_index;
    std::unordered_map<std::string, font_resource> font_index;
    int transient_id = 0;
};

}