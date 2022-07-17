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

#include "Character.hpp"

class Window;

class NPC : public Character
{
	public:
		NPC(const TileCoord &position, const std::string &filePath, const Map &map);
		COPY_CTOR(NPC) = delete;
		MOVE_CTOR(NPC) = delete;
		COPY_ASSIGN(NPC) = delete;
		MOVE_ASSIGN(NPC) = delete;
		~NPC();
		
		virtual bool move(const std::unique_ptr<SDL_Event> &event) override;
		
	private:
		/// Percentage chance that the NPC will move, if test fails the NPC
		/// stays still.
		static const int MOVE_CHANCE;
		
		/// Radius in which the NPC will perceive other Characters
		static const int SEARCH_RADIUS;
				
		bool try_flee();
		bool try_move_toward_enemy();
		bool try_attack_enemy();
		bool enemy_is_found();
		
		/// Searches for a nearby enemy.
		/// 
		/// If an enemy is found, the appropriate things are initialised
		/// and calls can be made to methods such as `can_move_toward_enemy()`
		/// or `attack_enemy()`.
		bool find_enemy();
		
		/// call this each time a new enemy is chosen
		void reset_enemy(const std::shared_ptr<Character> &);
		
		virtual void move_to_tile(const TileCoord &newPos) override;
		
		std::shared_ptr<Character> enemy;
};
