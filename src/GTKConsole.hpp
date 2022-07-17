#pragma once

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
