#include "Rect.hpp"

#include "Window.hpp"

Rect::Rect(Uint16 w, Uint16 h) :
	r(255), g(255), b(255),
	rect {0, 0, w, h}
{
	
}

Rect::~Rect() { }

void Rect::set_wh(Uint16 w, Uint16 h)
{
	rect.w = w; rect.h = h;
}

Uint16 Rect::get_w() const { return rect.w; }
Uint16 Rect::get_h() const { return rect.h; }

void Rect::set_rgb(Uint8 r_A, Uint8 g_A, Uint8 b_A)
{
	r = r_A;
	g = g_A;
	b = b_A;
}

void Rect::set_rgb(const SDL_Color &Color)
{
	r = Color.r;
	g = Color.g;
	b = Color.b;
}

void Rect::draw(const Window &window, Uint16 x, Uint16 y) const
{
	rect.x = x - window.get_camera_x();
	rect.y = y - window.get_camera_y();
	SDL_FillRect(window.get_screen(), &rect,
		SDL_MapRGB(window.get_screen()->format, r, g, b));
}

void Rect::draw_literal(const Window &window, Uint16 x, Uint16 y) const
{
	rect.x = x;
	rect.y = y;
	SDL_FillRect(window.get_screen(), &rect,
		SDL_MapRGB(window.get_screen()->format, r, g, b));
}
