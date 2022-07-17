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
