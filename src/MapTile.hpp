#pragma once

#include <list>
#include <functional>
#include "Character.hpp"

class Texture;
class Item;

class MapTile
{
	public:
		MapTile(bool solid, const std::shared_ptr<Texture> &);
		COPY_CTOR(MapTile) = delete;
		MOVE_CTOR(MapTile) = default;
		COPY_ASSIGN(MapTile) = delete;
		MOVE_ASSIGN(MapTile) = default;
		virtual ~MapTile();
		
		void draw(const Window &window, int, int) const;
		
		bool is_solid() const;
		bool is_walkable() const;
		bool character_is_present() const;
		
		      std::shared_ptr<Character> &get_character();
		const std::shared_ptr<Character> &get_character() const;
		const std::shared_ptr<Texture>   &get_texture()   const;
		
		// Moves the character currently on this tile to the specified tile.
		void move_character(MapTile &to);
		
		std::list<std::unique_ptr<Item>> &get_items();
		void add_item(std::unique_ptr<Item> &item);
		
		void set_solid    (bool);
		void set_character(const std::shared_ptr<Character> &);
		void set_texture  (const std::shared_ptr<Texture> &);
		void set_decal    (const std::shared_ptr<Texture> &);
		void set_trigger  (const std::function<void (MapTile &tile, Character &charr)> &trigger);
		
		// After a call to draw() this gets set to 0, so if the tile is meant to be illuminated this should be called every frame.
		// Accepts a number in the range of ILLUMINATION_MIN to ILLUMINATION_MAX.
		void set_illumination_level(int level);
		
		static const int ILLUMINATION_MIN;
		static const int ILLUMINATION_MAX;
		
	protected:
		bool solid;
		mutable int illumination;
		
		std::shared_ptr<Character> character;
		std::shared_ptr<Texture> texture;
		std::shared_ptr<Texture> decal;
		std::list<std::unique_ptr<Item>> items;
		
		// Called when a character visits this tile.
		std::function<void (MapTile &tile, Character &charr)> trigger;
};
