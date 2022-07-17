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
