#include "MapTile.hpp"

#include <stdexcept>
#include <SDL/SDL_gfxPrimitives.h>
#include "Item.hpp"
#include "Window.hpp"
#include "constants.hpp"

const int MapTile::ILLUMINATION_MIN = 0;
const int MapTile::ILLUMINATION_MAX = 100;

// -----------------------------------------------------------------------------
// Public
// -----------------------------------------------------------------------------
MapTile::MapTile(bool solid, const std::shared_ptr<Texture> &texture) :
	solid(solid),
	illumination(0),
	character(),
	texture(texture),
	decal(),
	items(),
	trigger(nullptr)
{
	
}

MapTile::~MapTile() {}

void MapTile::draw(const Window &window, int x, int y) const
{
	texture->draw(window, x, y);
	
	if (decal != nullptr)
		decal->draw(window, x, y);
	
	// boxRGBA takes a a coordinate pair indexed from the top left-hand side of the screen
	// rather than from the top left-hand side of the map area
	int xx = x - window.get_camera_x();
	int yy = y - window.get_camera_y();
	
	// // make it so that an illumination level of 0 is easily distinguishable from an illumination level of 1 or more
	int alpha = illumination == 0 ? 205 : 155 - illumination;
	
	boxRGBA(window.get_screen(), xx, yy, xx + TILE_WIDTH, yy + TILE_HEIGHT, 0, 0, 0, alpha);
	
	if (illumination != 0)
	{
		if (items.empty() == false)
		items.back()->draw(window, x, y);
	
		if (character_is_present())
			character->draw(window);
	}
	
	illumination = 0;
}

bool MapTile::is_solid() const
{ return solid; }

bool MapTile::is_walkable() const
{ return solid == false && character_is_present() == false; }

bool MapTile::character_is_present() const
{ return character != nullptr && character->is_alive() == true; }

std::shared_ptr<Character> &MapTile::get_character()
{ return character; }

const std::shared_ptr<Character> &MapTile::get_character() const
{ return character; }

const std::shared_ptr<Texture> &MapTile::get_texture() const
{ return texture; }

std::list<std::unique_ptr<Item>> &MapTile::get_items()
{ return items; }

void MapTile::add_item(std::unique_ptr<Item> &item)
{ items.emplace_back(std::move(item)); }

void MapTile::move_character(MapTile &to)
{
	to.set_character(character);
	character.reset();
}

void MapTile::set_solid(bool s)
{ solid = s; }

void MapTile::set_illumination_level(int level)
{
	if (level < MapTile::ILLUMINATION_MIN || level > MapTile::ILLUMINATION_MAX)
		throw std::runtime_error(LINEANDFILEB + "'" + std::to_string(level) + "'" + " was < ILLUMINATION_MIN || > ILLUMINATION_MAX");
	
	illumination = level;
}

void MapTile::set_character(const std::shared_ptr<Character> &c)
{
	if (is_walkable() == false)
		throw std::runtime_error(LINEANDFILEB + "cannot set character '" +
			character->get_stats().name + "' to tile (it could be" +
			"solid or already contain another character)");
	
	character = c;
	if (trigger != nullptr)
		trigger(*this, *character);
}

void MapTile::set_texture(const std::shared_ptr<Texture> &t)
{ texture = t; }

void MapTile::set_decal(const std::shared_ptr<Texture> &d)
{ decal = d; }

void MapTile::set_trigger(const std::function<void (MapTile &tile, Character &charr)> &t)
{ trigger = t; }
