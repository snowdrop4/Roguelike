#include "Tileset.hpp"

#include <dirent.h>
#include <iostream>
#include "Texture.hpp"
#include "StringUtils.hpp"
#include "constants.hpp"

// -----------------------------------------------------------------------------
// Static
// -----------------------------------------------------------------------------
static std::deque<std::string> get_files_in_dir(const std::string &dir)
{
	std::deque<std::string> filenames;
	
	DIR *dp;
	dirent *dirp;
	
	if ((dp = opendir(dir.c_str())) == nullptr)
		throw std::runtime_error(LINEANDFILEB + " error opening directory '" + dir + "'");
	
	while ((dirp = readdir(dp)) != nullptr)
	{
		if (strcmp(dirp->d_name, ".") != 0 && strcmp(dirp->d_name, "..") != 0)
			filenames.emplace_back(dirp->d_name);
	}
	
	closedir(dp);
	
	return filenames;
}

// -----------------------------------------------------------------------------
// Tileset - Public
// -----------------------------------------------------------------------------
Tileset::Tileset(const std::string &tilesetPath) :
	standard(),
	wall(),
	fancy(),
	topLeft(),
	topRight(),
	botLeft(),
	botRight(),
	leverOpen(),
	leverClosed()
{
	auto files = get_files_in_dir(tilesetPath);
	
	#define EMPLACE(arg) arg.emplace_back(new Texture(tilesetPath + i, false));
	for (auto &i : files)
	{
		if      (is_prefix_of("standard", i))    EMPLACE(standard)
		else if (is_prefix_of("wall", i))        EMPLACE(wall)
		else if (is_prefix_of("fancy", i))       EMPLACE(fancy)
		else if (is_prefix_of("topLeft", i))     EMPLACE(topLeft)
		else if (is_prefix_of("topRight", i))    EMPLACE(topRight)
		else if (is_prefix_of("botLeft", i))     EMPLACE(botLeft)
		else if (is_prefix_of("botRight", i))    EMPLACE(botRight)
		else if (is_prefix_of("leverOpen", i))   EMPLACE(leverOpen)
		else if (is_prefix_of("leverClosed", i)) EMPLACE(leverClosed)
		else std::cerr << LINEANDFILEA << "'" << tilesetPath << i << "' is not a valid tile\n";
	}
	#undef EMPLACE
}

Tileset::~Tileset() { }

#define RANDOM(arg) const { return arg[rand() % arg.size()]; }
const std::shared_ptr<Texture> &Tileset::get_standard() RANDOM(standard)
const std::shared_ptr<Texture> &Tileset::get_wall()	    RANDOM(wall)
const std::shared_ptr<Texture> &Tileset::get_fancy()    RANDOM(fancy)

const std::shared_ptr<Texture> &Tileset::get_top_left()  RANDOM(topLeft)
const std::shared_ptr<Texture> &Tileset::get_top_right() RANDOM(topRight)
const std::shared_ptr<Texture> &Tileset::get_bot_left()  RANDOM(botLeft)
const std::shared_ptr<Texture> &Tileset::get_bot_right() RANDOM(botRight)

const std::shared_ptr<Texture> &Tileset::get_lever_open()   RANDOM(leverOpen)
const std::shared_ptr<Texture> &Tileset::get_lever_closed() RANDOM(leverClosed)

// -----------------------------------------------------------------------------
// GenericTileset - Public
// -----------------------------------------------------------------------------
GenericTileset::GenericTileset(const std::string &tilesetPath) :
	blood(),
	black(),
	portalActive(),
	portalInactive(),
	chestOpen(),
	chestClosed()
{
	auto files = get_files_in_dir(tilesetPath);
	
	#define EMPLACE(arg) arg.emplace_back(new Texture(tilesetPath + i, Colors::textureColorKey));
	for (auto &i : files)
	{
		if      (is_prefix_of("blood", i)) EMPLACE(blood)
		else if (is_prefix_of("black", i)) EMPLACE(black)
		else if (is_prefix_of("portalActive", i))   EMPLACE(portalActive)
		else if (is_prefix_of("portalInactive", i)) EMPLACE(portalInactive)
		else if (is_prefix_of("chestOpen", i))      EMPLACE(chestOpen)
		else if (is_prefix_of("chestClosed", i))    EMPLACE(chestClosed)
		else std::cerr << LINEANDFILEA << "'" << tilesetPath << i << "' is not a valid tile\n";
	}
	#undef EMPLACE
}

GenericTileset::~GenericTileset() { }

const std::shared_ptr<Texture> &GenericTileset::get_blood() RANDOM(blood)
const std::shared_ptr<Texture> &GenericTileset::get_black() RANDOM(black)

const std::shared_ptr<Texture> &GenericTileset::get_portal_active()   RANDOM(portalActive)
const std::shared_ptr<Texture> &GenericTileset::get_portal_inactive() RANDOM(portalInactive)

const std::shared_ptr<Texture> &GenericTileset::get_chest_open()   RANDOM(chestOpen)
const std::shared_ptr<Texture> &GenericTileset::get_chest_closed() RANDOM(chestClosed)

#undef RANDOM
