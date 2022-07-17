#pragma once

#include <SDL/SDL_video.h>
#include <string>

const int    GRAPHICS_BASE_SIZE = 8;
const int    TEXT_SIZE          = 8;
const double GRAPHICS_SCALE     = 4.0;

const int TILE_WIDTH    = GRAPHICS_BASE_SIZE * GRAPHICS_SCALE;
const int TILE_HEIGHT   = GRAPHICS_BASE_SIZE * GRAPHICS_SCALE;
const int GRAPHICS_SIZE = GRAPHICS_BASE_SIZE * GRAPHICS_SCALE;

namespace Defaults
{
	const std::string itemPath    = "../resources/items/";
	const std::string charPath    = "../resources/characters/";
	const std::string tilesetPath = "../resources/tilesets/";
}

namespace Colors
{
	const SDL_Color brightWhite = { 255, 255, 255 };
	const SDL_Color white       = { 215, 215, 215 };
	const SDL_Color dullWhite   = { 175, 175, 175 };
	
	const SDL_Color lightGrey   = { 125, 125, 125 };
	const SDL_Color grey        = { 85,  85,  85  };
	const SDL_Color darkGrey    = { 45,  45,  45  };
	
	const SDL_Color black       = { 0,   0,   0   };
	
	const SDL_Color brightRed   = { 255, 20,  20  };
	const SDL_Color red         = { 150, 20,  20  };
	const SDL_Color dullRed     = { 80,  20,  20  };
	
	const SDL_Color brightGreen = { 20,  255, 20  };
	const SDL_Color green       = { 20,  150, 20  };
	const SDL_Color dullGreen   = { 80,  20,  20  };
	
	const SDL_Color textureColorKey = { 0, 255, 255 };
}

namespace messageColors
{
	const SDL_Color vGood     = { 30,  225, 30  };
	const SDL_Color good      = { 100, 225, 100 };
	const SDL_Color welcome   = { 100, 225, 225 };
	
	const SDL_Color neutral   = { 0,   0,   0   };
	
	const SDL_Color unwelcome = { 225, 225, 100 };
	const SDL_Color bad       = { 225, 100, 100 };
	const SDL_Color vBad      = { 225, 30,  30  };
}

namespace messagePrefixes
{
	const std::string combat  = "@ | "; // @ | You hit the rat for 30 damage
	const std::string action  = "# | "; // # | You pull the lever and hear a grinding sound in the distance
	const std::string player  = "? | "; // ? | You do not have the required stats to equip that item
	const std::string warning = "! | "; // ! | That destination is no longer reachable
}
