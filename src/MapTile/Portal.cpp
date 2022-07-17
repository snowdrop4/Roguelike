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

#include "Portal.hpp"

#include "../constants.hpp"
#include "../Item.hpp"
#include "../Player.hpp"

// -----------------------------------------------------------------------------
// Public
// -----------------------------------------------------------------------------
MapTilePortal::MapTilePortal(const std::shared_ptr<Texture> &portalActive,
		                     const std::shared_ptr<Texture> &portalInactive,
		                     const std::function<void ()>   &portalTriggerA) :
	MapTile(false, portalInactive),
	portalActive(portalActive),
	portalInactive(portalInactive),
	portalTrigger(portalTriggerA),
	active(false)
{
	set_trigger([&](MapTile &t, Character &charr)
		{
			if (charr.is_player())
			{
				if (active == true)
					portalTrigger();
				else
				{
					// print a game log message
					auto *p = (Player *) &charr;
					p->get_game_log() << messageColors::neutral << messagePrefixes::action
				                          << "You walk over the glowing and seemingly magical square but nothing seems to happen\n";
				}
			}
		});
}

MapTilePortal::~MapTilePortal() { }

void MapTilePortal::toggle_enabled()
{
	active = !active;
	active ? set_texture(portalActive) : set_texture(portalInactive);
}
