#pragma once

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
