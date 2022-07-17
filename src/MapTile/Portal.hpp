#pragma once


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
