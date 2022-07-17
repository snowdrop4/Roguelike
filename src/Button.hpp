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

#include <functional>
#include "Text.hpp"
#include "Rect.hpp"

class Button
{
	public:
		Button(const std::string &text, int x, int y,
               const std::function<void()> &callback);
		COPY_CTOR(Button) = default;
		MOVE_CTOR(Button) = default;
		COPY_ASSIGN(Button) = default;
		MOVE_ASSIGN(Button) = default;
		~Button();
		
		int get_x() const;
		int get_y() const;
		int get_w() const;
		int get_h() const;
		
		void draw        (const Window &window) const;
		void draw_literal(const Window &window) const;
		
		void set_out_rgb(Uint8, Uint8, Uint8);
		void set_out_rgb(const SDL_Color &);
		void set_over_rgb(Uint8, Uint8, Uint8);
		void set_over_rgb(const SDL_Color &);
		
		void set_text(const std::string &);
		void set_xy(int, int);
		
		void handle_input(const SDL_Event &event);
		
	private:
		std::function<void()> callback;
		
		bool mouseOver;
		
		SDL_Color mouseOutColour;
		SDL_Color mouseOverColour;
		
		Text text;
		Rect box;
		
		int x, y, w, h;
};
