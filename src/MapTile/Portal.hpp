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
#include "../MapTile.hpp"

class MapTilePortal : public MapTile
{
	public:
		MapTilePortal(const std::shared_ptr<Texture> &portalActive,
		              const std::shared_ptr<Texture> &portalInactive,
		              const std::function<void ()>   &trigger);
		~MapTilePortal();
	
	void toggle_enabled();
	
	private:
		const std::shared_ptr<Texture> &portalActive;
		const std::shared_ptr<Texture> &portalInactive;
		
		const std::function<void ()> portalTrigger;
		
		bool active;
};
