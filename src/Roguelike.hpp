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
#include "Window.hpp"
#include "UtilDefines.hpp"

class MainMenu;
class Game;

enum class RoguelikeState
{
	menu,
	game,
	quit
};

class Roguelike
{
	public:
		Roguelike();
		COPY_CTOR(Roguelike) = delete;
		MOVE_CTOR(Roguelike) = default;
		COPY_ASSIGN(Roguelike) = delete;
		MOVE_ASSIGN(Roguelike) = default;
		~Roguelike();
		
		void run();
		
	private:
		void draw() const;
		void update();
		
		void menu_initialise();
		void menu_unload();
		
		void game_initialise();
		void game_unload();
		
		Window window;
		
		RoguelikeState roguelikeState;
		
		std::unique_ptr<SDL_Event> event;
		std::unique_ptr<MainMenu>  menu;
		std::unique_ptr<Game>      game;
};
