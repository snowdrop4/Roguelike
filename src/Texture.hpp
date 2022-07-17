#pragma once

/*
 * Copyright 2012 Alyxr
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with This program.  If not, see <http://www.gnu.org/licenses/>.
 * 
*/

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
