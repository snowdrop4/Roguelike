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

#include "Chest.hpp"
 
#include "../constants.hpp"
#include "../Item.hpp"
#include "../Player.hpp"

// -----------------------------------------------------------------------------
// Public
// -----------------------------------------------------------------------------
MapTileChest::MapTileChest(const std::shared_ptr<Texture> &chestOpen, 
                           const std::shared_ptr<Texture> &chestClosed) :
	MapTile(false, chestClosed),
	chestOpen(chestOpen),
	opened(false)
{
	set_trigger([&](MapTile &t, Character &charr)
		{
			if (opened == false && charr.is_player())
			{
				// change state and change the texture accordingly
				opened = true;
				set_texture(chestOpen);
				
				// print a game log message
				auto *p = (Player *) &charr;
				p->get_game_log() << messageColors::neutral << messagePrefixes::action << "You open the chest to find...\n";
				
				// TODO: make the chest actually hold an item
				auto item = std::unique_ptr<Item>(new Item(Defaults::itemPath + "wearable/shieldWood"));
				add_item(item);
			}
		});
}

MapTileChest::~MapTileChest() {}
