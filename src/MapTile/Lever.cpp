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

#include "Lever.hpp"

#include "../constants.hpp"
#include "../Item.hpp"
#include "../Player.hpp"

// -----------------------------------------------------------------------------
// Public
// -----------------------------------------------------------------------------
MapTileLever::MapTileLever(const std::shared_ptr<Texture> &leverOpen,
                           const std::shared_ptr<Texture> &leverClosed,
                           const std::function<void ()>   &leverTriggerA) :
	MapTile(false, leverClosed),
	leverOpen(leverOpen),
	leverClosed(leverClosed),
	leverTrigger(leverTriggerA),
	open(false)
{
	set_trigger([&](MapTile &t, Character &charr)
		{
			if (charr.is_player())
			{
				// toggle state and change the texture accordingly
				open = !open;
				open ? set_texture(leverOpen) : set_texture(leverClosed);

				// fire the lever callback
				leverTrigger();
				
				// print a game log message
				std::string action = open ? "on" : "off";
				
				auto *p = (Player *) &charr;
				p->get_game_log() << messageColors::neutral << messagePrefixes::action
				                  << "You turn " + action + " the lever, you hear an unspecific noise an unknown distance away\n";
			}
		});
}

MapTileLever::~MapTileLever() { }
