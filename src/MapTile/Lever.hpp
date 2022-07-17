#pragma once

#include <memory>
#include "../MapTile.hpp"

class MapTileLever : public MapTile
{
	public:
		MapTileLever(const std::shared_ptr<Texture> &leverOpen,
		             const std::shared_ptr<Texture> &leverClosed,
		             const std::function<void ()> &trigger);
		~MapTileLever();
	
	private:
		const std::shared_ptr<Texture> &leverOpen;
		const std::shared_ptr<Texture> &leverClosed;
		
		std::function<void ()> leverTrigger;
		
		bool open;
};
