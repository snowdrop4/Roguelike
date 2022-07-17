#pragma once

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
