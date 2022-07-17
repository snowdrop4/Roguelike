#pragma once

// 
// Automatically loads textures in a tileset.
//
// The name of a directory should be given to the constructor, the directory is
// then scanned for any image that has one of the following as a prefix in
// its filename:
// 
//     standard
//     wall
//     fancy
//     topLeft
//     topRight
//     botLeft
//     botRight
//     portalDown
// 
// Multiple textures with the same prefix may be loaded (eg. you'd have
// something like wall1.png and wall2.png), in which case the 'get_' functions
// return a random texture from the set.
// 
// Tilesets should be found in ../recoures/tilesets/
// 

#include <deque>
#include <memory>
#include "UtilDefines.hpp"

class Texture;

class Tileset
{
	public:
		Tileset(const std::string &tilesetPath);
		COPY_CTOR(Tileset) = delete;
		MOVE_CTOR(Tileset) = delete;
		COPY_ASSIGN(Tileset) = delete;
		MOVE_ASSIGN(Tileset) = delete;
		~Tileset();
		
		// Returns a random texture in the set, if more than one is available.
		const std::shared_ptr<Texture> &get_standard() const;
		const std::shared_ptr<Texture> &get_wall()	   const;
		const std::shared_ptr<Texture> &get_fancy()	   const;
		
		const std::shared_ptr<Texture> &get_top_left()  const;
		const std::shared_ptr<Texture> &get_top_right() const;
		const std::shared_ptr<Texture> &get_bot_left()  const;
		const std::shared_ptr<Texture> &get_bot_right() const;
		
		const std::shared_ptr<Texture> &get_lever_open()   const;
		const std::shared_ptr<Texture> &get_lever_closed() const;
		
	private:
		std::deque<std::shared_ptr<Texture>> standard;
		std::deque<std::shared_ptr<Texture>> wall;
		std::deque<std::shared_ptr<Texture>> fancy;
		
		std::deque<std::shared_ptr<Texture>> topLeft;
		std::deque<std::shared_ptr<Texture>> topRight;
		std::deque<std::shared_ptr<Texture>> botLeft;
		std::deque<std::shared_ptr<Texture>> botRight;
		
		std::deque<std::shared_ptr<Texture>> leverOpen;
		std::deque<std::shared_ptr<Texture>> leverClosed;
};

// 
// Like Tileset, but loads things like blood splatter, chests, bookcases, etc.
// Things that are theme agnostic/generic.
// 
// Normally, the directory should be ../resources/tilesets/generic
// 
class GenericTileset
{
	public:
		GenericTileset(const std::string &tilesetPath);
		COPY_CTOR(GenericTileset) = delete;
		MOVE_CTOR(GenericTileset) = delete;
		COPY_ASSIGN(GenericTileset) = delete;
		MOVE_ASSIGN(GenericTileset) = delete;
		~GenericTileset();
		
		// Returns a random texture in the set, if more than one is available.
		const std::shared_ptr<Texture> &get_blood() const;
		const std::shared_ptr<Texture> &get_black() const;
		
		const std::shared_ptr<Texture> &get_portal_active()   const;
		const std::shared_ptr<Texture> &get_portal_inactive() const;
		
		const std::shared_ptr<Texture> &get_chest_open()   const;
		const std::shared_ptr<Texture> &get_chest_closed() const;
		
	private:
		std::deque<std::shared_ptr<Texture>> blood;
		std::deque<std::shared_ptr<Texture>> black;
		
		std::deque<std::shared_ptr<Texture>> portalActive;
		std::deque<std::shared_ptr<Texture>> portalInactive;
		
		std::deque<std::shared_ptr<Texture>> chestOpen;
		std::deque<std::shared_ptr<Texture>> chestClosed;
};
