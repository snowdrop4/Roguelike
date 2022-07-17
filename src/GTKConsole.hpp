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

#pragma GCC diagnostic ignored "-Weffc++"
#include <gtk/gtk.h>
#pragma GCC diagnostic pop
#include <string>
#include "UtilDefines.hpp"

struct SDL_Color;

class GTKConsole
{
	public:
		GTKConsole();
		COPY_CTOR(GTKConsole) = delete;
		MOVE_CTOR(GTKConsole) = default;
		COPY_ASSIGN(GTKConsole) = delete;
		MOVE_ASSIGN(GTKConsole) = default;
		~GTKConsole();
		
		void add_text(const std::string &str);
		
		GTKConsole &operator << (const std::string &str);
		GTKConsole &operator << (const SDL_Color   &col);
		
	private:
		GtkWidget     *window;
		GtkTextBuffer *textBuffer;
		GtkWidget     *textView;
		GtkWidget     *textScrolled;
		GtkTextTag    *textColor;
		GtkTextTag    *textStyle;
		GtkTextIter    textIter;
};
