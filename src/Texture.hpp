#pragma once

///
/// Interface for SDL_Surface and the load_image functions.
/// 
/// Automatically calls SDL_FreeSurface() when a texture is loaded over an old
/// one, or when the object is destructed.
///

#include <string>
#include "UtilDefines.hpp"

class Window;
struct SDL_Color;
struct SDL_Surface;

class Texture
{
	public:
		Texture();
		Texture(const std::string &, bool alpha);
		Texture(const std::string &, const SDL_Color &);
		Texture(const std::string &, Uint8 r, Uint8 g, Uint8 b);
		COPY_CTOR(Texture) = delete;
		MOVE_CTOR(Texture);
		COPY_ASSIGN(Texture) = delete;
		MOVE_ASSIGN(Texture);
		~Texture();
		
		std::string get_path() const;
		int get_w() const;
		int get_h() const;
		
		void draw        (const Window &window, int, int) const; /// takes into account the position of the camera
		void draw_literal(const Window &window, int, int) const; /// inverse of above
		
		void load(const std::string &);
		void load_alpha(const std::string &);
		
		void load_color_key(const std::string &, const SDL_Color &);
		void load_color_key(const std::string &, Uint8 r, Uint8 g, Uint8 b);
		
		void set_alpha(Uint8);
		
	private:
		SDL_Surface *texture;
		std::string texturePath;
};
