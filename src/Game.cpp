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

#include "Game.hpp"

#include "constants.hpp"
#include "Map.hpp"
#include "MapTile.hpp"
#include "Player.hpp"

// -----------------------------------------------------------------------------
// Public
// -----------------------------------------------------------------------------
Game::Game(Window &window, const std::unique_ptr<SDL_Event> &event) :
	map(),
	GTKThread(),
	cursor()
{
	gdk_threads_init();
	gtk_init(nullptr, nullptr);
	
	GTKThread.reset(new std::thread([&]()
	{
		gdk_threads_enter();
		gtk_main();
		gdk_threads_leave();
	}));
	
	map.reset(new Map(Defaults::tilesetPath + "stone/", 50, 50, window));
}

Game::~Game()
{
	gdk_threads_enter();
	gtk_main_quit();
	gdk_threads_leave();
	GTKThread->join();
}

void Game::draw(const Window &window) const
{
	map->draw(window);
	cursor.draw(window, *map);
}

GameState Game::update(const std::unique_ptr<SDL_Event> &event)
{
	if (map->get_players_remaining() == 0)
		return GameState::menu;
	
	cursor.update(event);
	map->update(event);
	
	return GameState::game;
}

// -----------------------------------------------------------------------------
// Private
// -----------------------------------------------------------------------------
