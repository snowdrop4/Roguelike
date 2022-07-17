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
/// Interface for the rectangle drawing capabilities of SDL.
///

#include <SDL/SDL.h>
#include "UtilDefines.hpp"

class Window;

class Rect
{
	public:
		Rect(Uint16 w, Uint16 h);
		COPY_CTOR(Rect) = default;
		MOVE_CTOR(Rect) = default;
		COPY_ASSIGN(Rect) = default;
		MOVE_ASSIGN(Rect) = default;
		~Rect();
		
		void set_wh(Uint16, Uint16);
		
		Uint16 get_w() const;
		Uint16 get_h() const;
		
		void set_rgb(Uint8, Uint8, Uint8);
		void set_rgb(const SDL_Color &);
		
		void draw(const Window &window, Uint16 x, Uint16 y) const;
		void draw_literal(const Window &window, Uint16 x, Uint16 y) const;
		
	private:
		Uint8 r, g, b;
		
		mutable SDL_Rect rect;
};
