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
#include <thread>
#include "UtilDefines.hpp"
#include "Cursor.hpp"

class GenericTileset;
class Map;
class Player;
class Window;
union SDL_Event;

enum class GameState
{
	menu,
	game,
	quit
};

class Game
{
	public:
		Game(Window &window, const std::unique_ptr<SDL_Event> &event);
		COPY_CTOR(Game) = delete;
		MOVE_CTOR(Game) = default;
		COPY_ASSIGN(Game) = delete;
		MOVE_ASSIGN(Game) = default;
		~Game();
		
		GameState update(const std::unique_ptr<SDL_Event> &event);
		void draw(const Window &window) const;
		
	private:
		std::unique_ptr<Map> map;
		
		std::unique_ptr<std::thread> GTKThread;
		
		Cursor cursor;
};
