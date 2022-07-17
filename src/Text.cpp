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
