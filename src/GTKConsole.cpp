#include "GTKConsole.hpp"

#include <stdexcept>
#include <SDL/SDL_video.h>
#include "StringUtils.hpp"

// -----------------------------------------------------------------------------
// Static
// -----------------------------------------------------------------------------
static bool disable_window_close (GtkWidget *w, GdkEvent *e, gpointer d)
{ return true; }

// -----------------------------------------------------------------------------
// Public
// -----------------------------------------------------------------------------
GTKConsole::GTKConsole() :
	window(),
	textBuffer(),
	textView(),
	textScrolled(),
	textColor(),
	textStyle(),
	textIter()
{
	gdk_threads_enter();
	
	window       = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	textBuffer   = gtk_text_buffer_new(nullptr);
	textView     = gtk_text_view_new_with_buffer(textBuffer);
	textScrolled = gtk_scrolled_window_new(NULL, NULL);
	textColor    = gtk_text_buffer_create_tag(textBuffer, nullptr, "foreground", "#000000",   nullptr);
	textStyle    = gtk_text_buffer_create_tag(textBuffer, nullptr, "family",     "Monospace", nullptr);
	
	gtk_window_set_default_size(GTK_WINDOW(window), 250, 200);
	gtk_window_set_title(GTK_WINDOW(window), "Game Log");
	gtk_window_set_deletable(GTK_WINDOW(window), false);
	g_signal_connect(window, "delete-event", G_CALLBACK(disable_window_close), nullptr);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	
	gtk_text_view_set_editable(GTK_TEXT_VIEW(textView), false);
	gtk_text_buffer_get_start_iter(textBuffer, &textIter);
	
	gtk_container_add(GTK_CONTAINER(textScrolled), textView);
	gtk_container_add(GTK_CONTAINER(window), textScrolled);
	
	gtk_widget_show_all(window);
	
	gdk_threads_leave();
}

GTKConsole::~GTKConsole()
{
	gdk_threads_enter();
	gtk_widget_destroy(window);
	gdk_threads_leave();
}

void GTKConsole::add_text(const std::string &str)
{
	gdk_threads_enter();
	gtk_text_buffer_insert_with_tags(textBuffer, &textIter, str.c_str(), -1, textColor, textStyle, nullptr);
	gtk_text_view_scroll_to_iter(GTK_TEXT_VIEW(textView), &textIter, 0, false, 0, 0);
	gdk_threads_leave();
}

GTKConsole &GTKConsole::operator << (const std::string &str)
{
	add_text(str);
	return *this;
}

GTKConsole &GTKConsole::operator << (const SDL_Color &col)
{
	gdk_threads_enter();
	
	auto tagName = ("#" + uint8_to_hex_str(col.r) + uint8_to_hex_str(col.g) +
		uint8_to_hex_str(col.b)).c_str();
	
	auto tagTable = gtk_text_buffer_get_tag_table(textBuffer);
	
	textColor = gtk_text_tag_table_lookup(tagTable, tagName);
	if (textColor == nullptr)
		textColor = gtk_text_buffer_create_tag(textBuffer, nullptr, "foreground", tagName, nullptr);
	
	gdk_threads_leave();
	return *this;
}
