#include "Button.hpp"

#include "constants.hpp"

Button::Button(const std::string &text_A, int x, int y,
               const std::function<void()> &callback) :
	callback(callback),
	mouseOver(false),
	mouseOutColour(),
	mouseOverColour(),
	text(text_A, Colors::black),
	box(text.get_w() + 20, text.get_h() + 20),
	x(x), y(y),
	w(box.get_w()), h(box.get_h())
{
	set_out_rgb(Colors::dullWhite);
	set_over_rgb(Colors::white);
}

Button::~Button() { }

int Button::get_x() const
{ return x; }

int Button::get_y() const
{ return y; }

int Button::get_w() const
{ return w; }

int Button::get_h() const
{ return h; }

void Button::draw(const Window &window) const
{
	box.draw (window, x, y);
	text.draw(window, x + 10, y + 10);
}

void Button::draw_literal(const Window &window) const
{
	box.draw_literal (window, x, y);
	text.draw_literal(window, x + 10, y + 10);
}

void Button::set_out_rgb(Uint8 r, Uint8 g, Uint8 b)
{
	box.set_rgb(r, g, b);
	mouseOutColour = {r, g, b};
}

void Button::set_out_rgb(const SDL_Color &colour)
{
	box.set_rgb(colour);
	mouseOutColour = colour;
}

void Button::set_over_rgb(Uint8 r, Uint8 g, Uint8 b)
{ mouseOverColour = {r, g, b}; }

void Button::set_over_rgb(const SDL_Color &colour)
{ mouseOverColour = colour; }

void Button::set_text(const std::string &newText)
{
	text.set_text(newText, Colors::black);
	
	w = text.get_w() + 20;
	h = text.get_h() + 15;
	box.set_wh(w, h);
}

void Button::set_xy(int xArg, int yArg)
{
	x = xArg;
	y = yArg;
}

void Button::handle_input(const SDL_Event &event)
{
	if (event.type == SDL_MOUSEMOTION)
	{
		if (event.motion.x > x                &&
			event.motion.x < x + box.get_w()  &&
			event.motion.y > y                &&
			event.motion.y < y + box.get_h())
		{
			if (mouseOver == false)
			{
				box.set_rgb(mouseOverColour);
				box.set_wh(box.get_w() + 5, box.get_h());
				set_xy(x - 5, y);
			}
			mouseOver = true;
		}
		else
		{
			if (mouseOver == true)
			{
				box.set_rgb(mouseOutColour);
				box.set_wh(box.get_w() - 5, box.get_h());
				set_xy(x + 5, y);
			}
			mouseOver = false;
		}
	}
	else if (mouseOver == true &&
	         event.type == SDL_MOUSEBUTTONUP &&
		     event.button.button == SDL_BUTTON_LEFT)
		callback();
}
