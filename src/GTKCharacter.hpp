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

#include "UtilDefines.hpp"

#include <gtk/gtk.h>
#include <string>

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
