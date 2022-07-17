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

#include <memory>
#include <vector>
#include "UtilDefines.hpp"

class Window;
class Button;
class Text;
union SDL_Event;

enum class MenuState
{
	menu,
	game,
	quit
};

class MainMenu
{
	public:
		MainMenu(const Window &window);
		COPY_CTOR(MainMenu) = delete;
		MOVE_CTOR(MainMenu) = default;
		COPY_ASSIGN(MainMenu) = delete;
		MOVE_ASSIGN(MainMenu) = default;
		~MainMenu();
		
		MenuState update(const std::unique_ptr<SDL_Event> &event);
		void draw(const Window &window);
		
	private:
		void draw_background (const Window &window);
		void reset_background(const Window &window);
		
		// callbacks for the buttons
		void btn_new();
		void btn_load();
		void btn_options();
		void btn_quit();
		
		void construct();
		void destruct();
		
		static const int MARGIN;
		
		std::vector<std::vector<int>> bg;
		std::unique_ptr<Text> title;
		std::vector<Button> buttons;
		
		MenuState ret;
};
