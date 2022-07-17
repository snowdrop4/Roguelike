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
