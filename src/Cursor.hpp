#pragma once

///
/// Cursor graphic that runs on a grid of size TILE_WIDTH, TILE_HEIGHT.
///

#include <memory>
#include "Texture.hpp"

union SDL_Event;
class Window;
class TileCoord;
class Map;

TileCoord mouse_xy_to_tile_coord(int x, int y, const Window &window);

class Cursor
{
	public:
		Cursor();
		COPY_CTOR(Cursor) = delete;
		MOVE_CTOR(Cursor) = default;
		COPY_ASSIGN(Cursor) = delete;
		MOVE_ASSIGN(Cursor) = delete;
		~Cursor();

		TileCoord get_tile_coord(const Window &window) const;
		
		void draw(const Window &window, const Map &map) const;
		
		/// true  : mouse was moved (and the cursor points to a different tile)
		/// false : mouse was not moved/mouse was moved but cursor still points
		///         to same tile
		bool update(const std::unique_ptr<SDL_Event> &event);
		
	private:
		Texture texture;
		int x, y;
};
