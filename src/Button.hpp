#pragma once

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
