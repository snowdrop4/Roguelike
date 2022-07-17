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
/// Interface for stringRGBA(...) from SDL_gfx.
///

#include <SDL/SDL.h>
#include <string>
#include <deque>
#include "UtilDefines.hpp"

class Window;

class Text
{
	public:
		Text(const std::string &, const SDL_Color &);
		Text(const std::string &, Uint8 r, Uint8 g, Uint8 b);
		COPY_CTOR(Text) = default;
		MOVE_CTOR(Text) = default;
		COPY_ASSIGN(Text) = default;
		MOVE_ASSIGN(Text) = default;
		~Text();

		int get_w() const;
		int get_h() const;
		
		std::string get_text() const;
		
		void draw        (const Window &window, int, int) const;
		void draw_literal(const Window &window, int, int) const;
		
		void set_text   (const std::string &t, const SDL_Color &c);
		void set_text   (const std::string &t, Uint8 r, Uint8 g, Uint8 b);
		void append_text(const std::string &t, const SDL_Color &c);
		void append_text(const std::string &t, Uint8 r, Uint8 g, Uint8 b);
		
	private:
		int width;
		
		std::deque<std::string> text;
		std::deque<SDL_Color>   color;
};
