#include "NPC.hpp"

#include "Map.hpp"
#include "MapTile.hpp"

// -----------------------------------------------------------------------------
// Static
// -----------------------------------------------------------------------------
const int NPC::MOVE_CHANCE   = 80;
const int NPC::SEARCH_RADIUS = 10;

// -----------------------------------------------------------------------------
// Public
// -----------------------------------------------------------------------------
NPC::NPC(const TileCoord &position, const std::string &filePath, const Map &map) :
	Character(position, filePath, map),
	enemy(nullptr)
{
	
}

NPC::~NPC() { }

bool NPC::move(const std::unique_ptr<SDL_Event> &event)
{
	if (enemy_is_found() == true)
	{
		//~ if (should_flee() == true && try_flee() == true)
			//~ return true;
	}
	else if (find_enemy() == false)
		return false;
	
	try_move_toward_enemy() || try_attack_enemy();
	
	return true;
}

// -----------------------------------------------------------------------------
// Private
// -----------------------------------------------------------------------------
//bool NPC::try_flee()
//{
//	if (stats->hp.to_percentage() > 30)
//		return false;
//	
//	TileCoord bestPosition = position;
//	
//	for (int q = -1; q < 2; ++q)
//	for (int w = -1; w < 2; ++w)
//	{
//		 
//	}
//	
//	return true;
//}

// -----------------------------------------------------------------------------

bool NPC::try_move_toward_enemy()
{
	if (map.line_of_sight(position, enemy->get_xy()) == false)
	{
		enemy = nullptr;
		return false;
	}
	
	// if the tile is walkable, or contains the character we're looking for
	auto valid_tile = [&](const MapTile &tile, int xDiff, int yDiff) -> bool
	{
		if (tile.get_character() == enemy)
			return true;
		
		return tile.is_walkable();
	};
	
	auto path = std::move(map.get_path(position, enemy->get_xy(), valid_tile));
	
	// discard unneeded points
	path->pop_front(); // current position
	path->pop_back();  // end position (aka. destination character)
	
	if (path->size() > 0)
	{
		move_to_tile(path->front());
		return true;
	}
	return false;
}

// -----------------------------------------------------------------------------

bool NPC::try_attack_enemy()
{
	if (enemy == nullptr)
		return false;
	
	if (position.distance_from(enemy->get_xy()) == 1)
	{
		enemy->be_melee_attacked(*this);
		return true;
	}
	return false;
}

// -----------------------------------------------------------------------------

bool NPC::enemy_is_found()
{
	if (enemy == nullptr)
		return false;
	
	if (enemy->is_alive() == false)
	{
		enemy = nullptr;
		return false;
	}
	
	return true;
}

bool NPC::find_enemy()
{
	const auto &chars = map.characters_in_area(position, SEARCH_RADIUS);
	
	for (auto &i : chars)
		if (i->get_stats().faction != stats->faction &&
			map.line_of_sight(position, i->get_xy()))
		{
			enemy = i;
			return true;
		}
	
	return false;
}

// -----------------------------------------------------------------------------

void NPC::move_to_tile(const TileCoord &newPos)
{
	if (MOVE_CHANCE >= rand() % 100)
		Character::move_to_tile(newPos);
}
