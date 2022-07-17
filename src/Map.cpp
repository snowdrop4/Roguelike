#include "Map.hpp"

#include "Window.hpp"
#include "MapTile.hpp"
#include "MapTile/Lever.hpp"
#include "MapTile/Portal.hpp"
#include "MapTile/Chest.hpp"
#include "Item.hpp"
#include "CaveGenerator.hpp"
#include "Player.hpp"
#include "NPC.hpp"
#include "Lighting.hpp"

// -----------------------------------------------------------------------------
// Public
// -----------------------------------------------------------------------------
Map::Map(const std::string &tilesetPathA, int width, int height, Window &window) :
	cols(),
	rows(),
	tilesetPath(tilesetPathA),
	tileset(tilesetPath),
	genericTileset(Defaults::tilesetPath + "generic/"),
	astar(tiles, 0, 0),
	tiles(),
	floorTiles(),
	npcs(),
	player(),
	playerTurn(),
	regenerationRequested(false),
	window(window)
{
	regenerate(tilesetPath, width, height);
}

Map::~Map() { }

void Map::regenerate(const std::string &tilesetPathA, int width, int height)
{
	cols = width;
	rows = height;
	astar.resize(cols, rows);
	
	tilesetPath = tilesetPathA;
	// tileset(tilesetPath);
	
	npcs.clear();
	player.reset();
	
	regenerationRequested = false;
	playerTurn = true;
	
	tiles.clear();
	tiles.resize(rows);
	floorTiles.clear();
	
	create_tiles();
	populate_characters();
	populate_chests();
	populate_portal();
}

MapTile &Map::at(const TileCoord &a) const { return *(tiles[a.y][a.x]); }

int Map::get_tile_w()  const { return cols; }
int Map::get_tile_h()  const { return rows; }
int Map::get_pixel_w() const { return cols * TILE_WIDTH;  }
int Map::get_pixel_h() const { return rows * TILE_HEIGHT; }

int Map::get_players_remaining() const { return player->is_alive(); }
int Map::get_npcs_remaining()    const { return npcs.size(); }

std::deque<std::shared_ptr<Character>>
Map::characters_in_area(const TileCoord &origin, int radius) const
{
	std::deque<std::shared_ptr<Character>> ret;
	
	auto isInArea = [&](const TileCoord &t) -> bool
		{
			return (t != origin &&
				t.x >= origin.x - radius && t.x <= origin.x + radius &&
				t.y >= origin.y - radius && t.y <= origin.y + radius);
		};
	
	for (auto &i : npcs)
	{
		if (isInArea(i.second->get_xy()))
			ret.push_back(i.second);
	}
	
	if (player->is_alive() && isInArea(player->get_xy()))
		ret.push_back(player);
	
	return ret;
}

bool Map::line_of_sight(TileCoord first, TileCoord second) const
{
	// if x1 == x2 or y1 == y2, then it does not matter what we set here
	int deltaX = second.x - first.x;
	signed char ix((deltaX > 0) - (deltaX < 0));
	deltaX = abs(deltaX) << 1;
	
	int deltaY = second.y - first.y;
	signed char iy((deltaY > 0) - (deltaY < 0));
	deltaY = abs(deltaY) << 1;
	
	if (deltaX >= deltaY)
	{
		// error may go below zero
		int error(deltaY - (deltaX >> 1));
		
		while (first.x != second.x)
		{
			if ((error >= 0) && (error || (ix > 0)))
			{
				first.y += iy;
				error -= deltaX;
			}
			
			first.x += ix;
			error += deltaY;
			
			if (tiles[first.y][first.x]->is_solid())
				return false;
		}
	}
	else
	{
		// error may go below zero
		int error(deltaX - (deltaY >> 1));
		
		while (first.y != second.y)
		{
			if ((error >= 0) && (error || (iy > 0)))
			{
				first.x += ix;
				error -= deltaY;
			}
			
			first.y += iy;
			error += deltaX;
			
			if (tiles[first.y][first.x]->is_solid())
				return false;
		}
	}
	
	return true;
}

std::unique_ptr<std::list<std::pair<int, int>>>
	Map::get_path(const TileCoord &start, const TileCoord &end,
		std::function<bool (const MapTile &, int xDiff, int yDiff)> valid_tile,
		std::function<int  (const MapTile &, int xDiff, int yDiff)> move_cost,
		std::function<int  (int x1, int y1, int x2, int y2)> dist_cost) const
{
	// Promote functions that take a MapTile to functions that take a MapTile
	// enclosed by a unique_ptr. This allows us to 	
	auto promote_valid_tile = [&](const std::unique_ptr<MapTile> &t, int xDiff, int yDiff) -> bool
	{ return valid_tile(*t, xDiff, yDiff); };
	
	auto promote_move_cost = [&](const std::unique_ptr<MapTile> &t, int xDiff, int yDiff) -> int
	{ return move_cost(*t, xDiff, yDiff); };
	
	return std::move(astar.get_path(start.x, start.y, end.x, end.y,
		promote_valid_tile, promote_move_cost, dist_cost));
}

void Map::draw(const Window &window) const
{
	cast_light(*this, player->get_xy(), 5);
	
	int x1 = window.get_camera_x() / TILE_WIDTH;
	int y1 = window.get_camera_y() / TILE_HEIGHT;
	
	x1 = std::max(x1, 0);
	y1 = std::max(y1, 0);
	
	int x2 = (window.get_camera_x() + window.get_w()) / TILE_WIDTH;
	int y2 = (window.get_camera_y() + window.get_h()) / TILE_HEIGHT;
	
	x2 = std::min(x2, get_tile_w());
	y2 = std::min(y2, get_tile_h());
	
	for (int i = y1; i < y2; i++)
	for (int j = x1; j < x2; j++)
		tiles[i][j]->draw(window, j * TILE_WIDTH, i * TILE_HEIGHT);
}

void Map::update(const std::unique_ptr<SDL_Event> &event)
{
	if (regenerationRequested)
		regenerate(tilesetPath, cols, rows);
	
	if (player->is_alive() && playerTurn == true)
	{
		if (player->move(event) == true)
			playerTurn = false;
	}
	else
	{	
		for (auto i = npcs.begin(); i != npcs.end(); ++i)
		{
			// if the character is dead remove it from memory, else make its turn
			if (i->second->is_alive() == false)
			{
				at(i->second->get_xy()).set_decal(genericTileset.get_blood());
				npcs.erase(i);
			}
			else
				i->second->move(event);
		}
		
		playerTurn = true;
	}
}

// -----------------------------------------------------------------------------
// Private
// -----------------------------------------------------------------------------
TileCoord Map::get_rand_floor_tile()
{
	size_t at = rand() % floorTiles.size();
	
	auto it = floorTiles.begin();
	for (size_t i = 0; i < at; ++i)
		++it;
	
	auto coord = *it;
	floorTiles.erase(it);
	
	return coord;
}

void Map::create_tiles()
{
	CaveGenerator cave(cols, rows);
	cave.generate(50);
	
	auto coord_valid_and_floor = [&](int x_A, int y_A) -> bool
	{
		return (x_A > 0 && y_A > 0 && x_A < cols && y_A < rows) &&
			cave.get_type(x_A, y_A) == CellType::floor;
	};
	
	for (int i = 0; i < rows; ++i)
	{
		tiles[i].reserve(cols);
		for (int j = 0; j < cols; ++j)
		{
			// Create a wall tile if the tile is adjacent to one or more floor
			// tiles, or a void (black) tile if it is not.
			// 
			// If the wall tile tile is adjacent to only one floor tile make
			// it a sloped wall tile.
			if (cave.get_type(j, i) != CellType::floor)
			{
				int floorNeighbors = cave.get_floor_neighbors(j, i);
				
				if (floorNeighbors == 0)
					tiles[i].emplace_back(new MapTile(true, genericTileset.get_black()));
				else if (floorNeighbors == 1)
				{
					if      (coord_valid_and_floor(j - 1, i - 1)) tiles[i].emplace_back(new MapTile(true, tileset.get_top_left()));
					else if (coord_valid_and_floor(j + 1, i - 1)) tiles[i].emplace_back(new MapTile(true, tileset.get_top_right()));
					else if (coord_valid_and_floor(j - 1, i + 1)) tiles[i].emplace_back(new MapTile(true, tileset.get_bot_left()));
					else if (coord_valid_and_floor(j + 1, i + 1)) tiles[i].emplace_back(new MapTile(true, tileset.get_bot_right()));
					else                                          tiles[i].emplace_back(new MapTile(true, tileset.get_wall()));
				}
				else tiles[i].emplace_back(new MapTile(true, tileset.get_wall()));
			}
			else // else it's a floor tile
			{
				auto randomFloorTexture = (rand() % 100 > 75) ? tileset.get_fancy() : tileset.get_standard();
				
				tiles[i].emplace_back(new MapTile(false, randomFloorTexture));
				floorTiles.emplace_back(j, i);
			}
		}
	}
}

void Map::populate_characters()
{
	// player
	add_player<Player>(get_rand_floor_tile(), Defaults::charPath + "human/knight", *this, window);
	
	// npcs
	for (int i = 0; i < 1; ++i)
		add_npc<NPC>(get_rand_floor_tile(), Defaults::charPath + "monster/jellyCube",  *this);
}

void Map::populate_chests()
{
	for (int i = 0; i < 3; ++i)
	{
		auto chestTile = get_rand_floor_tile();
		tiles[chestTile.y][chestTile.x].reset(new MapTileChest(
			genericTileset.get_chest_open(), genericTileset.get_chest_closed()));
	}
}

void Map::populate_portal()
{
	// create portal down
	auto portalTile = get_rand_floor_tile();
	tiles[portalTile.y][portalTile.x].reset(new MapTilePortal(
		genericTileset.get_portal_active(), genericTileset.get_portal_inactive(),
		[&]()
		{
			// since a map regeneration requires the deletion of all Characters,
			// and it's Characters that trigger a regeneration we must delay
			// the regeneration until all Characters have finished moving
			// to prevent wickedness.
			regenerationRequested = true;
		}));
	
	// add lever
	auto leverTile = get_rand_floor_tile();
	tiles[leverTile.y][leverTile.x].reset(new MapTileLever(
		tileset.get_lever_open(), tileset.get_lever_closed(),
		[this, portalTile]()
		{
			auto portalPointer = (MapTilePortal *) &at(portalTile);
			portalPointer->toggle_enabled();
		}));
}
