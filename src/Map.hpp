#pragma once

#include "Tileset.hpp"
#include "AStar.hpp"
#include <stdexcept>
#include <functional>

class Window;
class Character;
class NPC;
class Player;
class TileCoord;
class MapTile;
union SDL_Event;

class Map
{
	public:
		Map(const std::string &tilesetPath, int width, int height, Window &window);
		COPY_CTOR(Map) = delete;
		MOVE_CTOR(Map) = delete;
		COPY_ASSIGN(Map) = delete;
		MOVE_ASSIGN(Map) = delete;
		~Map();
		
		void regenerate(const std::string &tilesetPath, int width, int height);
		
		MapTile &at(const TileCoord &) const;
		
		int get_tile_w() const;
		int get_tile_h() const;
		int get_pixel_w() const;
		int get_pixel_h() const;
		
		int get_players_remaining() const;
		int get_npcs_remaining()    const;
		
		std::deque<std::shared_ptr<Character>> characters_in_area(const TileCoord &, int radius) const;
		
		bool line_of_sight(TileCoord first, TileCoord second) const;
		
		std::unique_ptr<std::list<std::pair<int, int>>>
			get_path(const TileCoord &start, const TileCoord &end,
				std::function<bool (const MapTile &, int xDiff, int yDiff)> valid_tile,
				std::function<int  (const MapTile &, int xDiff, int yDiff)> move_cost = AStarFuncs::move_cost<MapTile>,
				std::function<int  (int x1, int y1, int x2, int y2)>        dist_cost = AStarFuncs::dist_cost) const;
		
		void draw(const Window &window) const;
		
		/// Calls 'get_move()' on all the Characters in the map in order of
		///  their initiative stat, then makes the appropriate move.
		/// 
		/// Removes dead Characters from memory.
		void update(const std::unique_ptr<SDL_Event> &event);
		
		template<typename T, typename...Args> void add_npc(Args&&... args);
		template<typename T, typename...Args> void add_player(Args&&... args);
		
	private:
		template<typename T> void add_character(std::shared_ptr<T> &);
		
		TileCoord get_rand_floor_tile();
		
		void create_tiles();
		void populate_characters();
		void populate_chests();
		void populate_portal();
		
		int cols;
		int rows;
		
		std::string tilesetPath;
		
		Tileset tileset;
		GenericTileset genericTileset;
		
		AStar<std::unique_ptr<MapTile>> astar;
		std::vector<std::vector<std::unique_ptr<MapTile>>> tiles;
		
		std::list<TileCoord> floorTiles;
		
		std::multimap<int, std::shared_ptr<Character>> npcs;
		std::shared_ptr<Player> player;
		
		bool playerTurn;
		bool regenerationRequested;
		
		Window &window;
};

template<typename T, typename...Args>
void Map::add_npc(Args&&... args)
{
	auto charr = std::make_shared<T>(std::forward<Args>(args)...);
	add_character<T>(charr);
	npcs.insert(std::make_pair(charr->get_stats().initiative, charr));
}

template<typename T, typename...Args>
void Map::add_player(Args&&... args)
{
	player = std::make_shared<T>(std::forward<Args>(args)...);
	add_character<T>(player);
}

template<typename T>
void Map::add_character(std::shared_ptr<T> &charr)
{
	auto xy = charr->get_xy();
	
	if (xy.is_in_bounds(*this) == false)
		throw std::runtime_error(LINEANDFILEB + "character named \"" + charr->get_stats().name +
		       "\" added to invalid map position " + xy.to_string());
	
	tiles[xy.y][xy.x]->set_character(charr);
}
