#pragma once

#include "UtilDefines.hpp"

#include <gtk/gtk.h>
#include <string>
#include <functional>

class Character;

struct lblPair
{
	GtkWidget *lbl;
	GtkWidget *val;
};

struct lblBarPair
{
	GtkWidget *lbl;
	GtkWidget *bar;
};

class GTKCharacter
{
	public:
		GTKCharacter(Character &charr);
		COPY_CTOR(GTKCharacter) = delete;
		MOVE_CTOR(GTKCharacter) = default;
		COPY_ASSIGN(GTKCharacter) = delete;
		MOVE_ASSIGN(GTKCharacter) = default;
		~GTKCharacter();
		
		// Re-reads the character data and updates the window.
		void update();
		
	private:
		// Private version of 'update()'. Does not contain 'gdk_threads_*()'
		// calls so as to prevent something bad happening when called from 
		// inside a context where 'gdk_threads_*()'' has already been called.
		void update_private();
		
		lblPair new_label_pair(const std::string &str);
		lblBarPair new_labeled_styled_level_bar(const std::string &cssPath);
		void new_seperator();
		
		int y;
		
		Character &charr;
		
		GtkWidget *window;
		GtkWidget *grid;
		
		lblPair name;
		lblPair faction;
		lblPair level;
		
		lblBarPair hp;
		lblBarPair mp;
		lblBarPair wt;
		lblBarPair xp;
		
		lblPair strength;
		lblPair intelligence;
		lblPair dexterity;
		lblPair constitution;
		lblPair initiative;
		
		GtkWidget *inventoryView;
		GtkWidget *inventoryScrolled;
		GtkListStore *inventoryItems;
};
