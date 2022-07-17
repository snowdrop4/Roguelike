#include "Text.hpp"

#include <SDL/SDL_gfxPrimitives.h>
#include "Window.hpp"

// -----------------------------------------------------------------------------
// Public
// -----------------------------------------------------------------------------
Text::Text(const std::string &t, const SDL_Color &c) :
	width(TEXT_SIZE * t.length()),
	text(),
	color()
{
	text.push_back(t);
	color.push_back(c);
}

Text::Text(const std::string &t, Uint8 r, Uint8 g, Uint8 b) :
	width(TEXT_SIZE * t.length()),
	text(),
	color()
{
	text.push_back(t);
	color.push_back({r, g, b});
}

Text::~Text() { }

int Text::get_w() const
{ return width; }

int Text::get_h() const
{ return TEXT_SIZE; }

std::string Text::get_text() const
{
	std::string ret;
	for (auto &i : text)
		ret += i;
	return ret;
}

void Text::draw(const Window &window, int x, int y) const
{
	for (size_t i = 0; i < text.size(); ++i)
	{
		stringRGBA(window.get_screen(), 
			x - window.get_camera_x(),
			y - window.get_camera_y(),
			text[i].c_str(),
			color[i].r, color[i].g, color[i].b, 255);
		
		x += TEXT_SIZE * text[i].length();
	}
}

void Text::draw_literal(const Window &window, int x, int y) const
{ draw(window, x + window.get_camera_x(), y + window.get_camera_y()); }

void Text::append_text(const std::string &t, const SDL_Color &c)
{
	text.push_back(t);
	color.push_back(c);
	width += TEXT_SIZE * t.length();
}

void Text::append_text(const std::string &t, Uint8 r, Uint8 g, Uint8 b)
{ append_text(t, {r, g, b}); }

void Text::set_text(const std::string &t, const SDL_Color &c)
{
	text.clear();
	color.clear();
	
	text.push_back(t);
	color.push_back(c);
	width = TEXT_SIZE * t.length();
}

void Text::set_text(const std::string &t, Uint8 r, Uint8 g, Uint8 b)
{ set_text(t, {r, g, b}); }
