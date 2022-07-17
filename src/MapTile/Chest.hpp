#pragma once

#include <memory>
#include "../MapTile.hpp"

class Texture;

class MapTileChest : public MapTile
{
	public:
		MapTileChest(const std::shared_ptr<Texture> &chestOpen,
		             const std::shared_ptr<Texture> &chestClosed);
		~MapTileChest();
	
	private:
		const std::shared_ptr<Texture> &chestOpen;
		
		bool opened;
};
