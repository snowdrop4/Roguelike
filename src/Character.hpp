#pragma once

#include <memory>
#include <list>
#include "Texture.hpp"
#include "TileCoord.hpp"
#include "Stats.hpp"

class Window;
class Map;
class MapTile;
class Text;
class Item;
union SDL_Event;

class Character
{
	public:
		Character(const TileCoord &position, const std::string &configFilePath,
		          const Map &map);
		COPY_CTOR(Character) = delete;
		MOVE_CTOR(Character) = default;
		COPY_ASSIGN(Character) = delete;
		MOVE_ASSIGN(Character) = default;
		virtual ~Character();
		
		virtual void draw(const Window &window) const;
		
		MapTile &get_map_tile() const;
		const TileCoord &get_xy() const;
		const Stats::Character &get_stats() const;
		const Stats::Abilities &get_abilities() const;
		const std::list<std::unique_ptr<Item>> &get_inventory() const;
		
		bool is_alive() const;
		virtual bool is_player() const;
		
		// return : if the character moved
		virtual bool move(const std::unique_ptr<SDL_Event> &event) = 0;
		
		// return : the amount of damage caused by the attacker
		virtual int be_melee_attacked(const Character &by);
		//virtual int be_range_attacked(const Character &by);
		//virtual int be_magic_attacked(const Character &by);
		
	protected:
		// Should only be used inside a move method with its own logic,
		// doesn't test to make sure that the destination tile is a valid one.
		// 
		// Just used to encapsulate the tile code.
		virtual void move_to_tile(const TileCoord &newPos);
		
		// Removes '*this' from the tile it resides in.
		// 
		// Before the Map tells a character to make a move, it checks whether
		// the character is dead -- if so it will be deleted. This method
		// only removes this character from the view of other characters,
		// and the game; this character will still exist in memory until
		// the Map deletes it.
		virtual void handle_death();
		
		static bool astar_valid_tile(const MapTile &, int xDiff, int yDiff);
		
		const Map &map;
		Texture texture;
		
		TileCoord position;
		
		std::unique_ptr<Stats::Character> stats;
		std::unique_ptr<Stats::Abilities> abilities;
		std::list<std::unique_ptr<Item>> inventory;
};
