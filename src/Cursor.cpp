#include "Cursor.hpp"

#include <SDL/SDL_events.h>
#include "Window.hpp"
#include "TileCoord.hpp"
#include "Map.hpp"
#include "MapTile.hpp"
#include "Text.hpp"

// -----------------------------------------------------------------------------
// Funcs
// -----------------------------------------------------------------------------
TileCoord mouse_xy_to_tile_coord(int x, int y, const Window &window)
{
	return { x / TILE_WIDTH  + window.get_camera_x() / TILE_WIDTH,
             y / TILE_HEIGHT + window.get_camera_y() / TILE_HEIGHT };
}

// -----------------------------------------------------------------------------
// Public
// -----------------------------------------------------------------------------
Cursor::Cursor() :
	texture(),
	x(0), y(0)
{
	texture.load_color_key("../resources/cursor.png", Colors::textureColorKey);
}

Cursor::~Cursor() { }

TileCoord Cursor::get_tile_coord(const Window &window) const
{ return mouse_xy_to_tile_coord(x, y, window); }

void Cursor::draw(const Window &window, const Map &map) const
{
	texture.draw_literal(window, x, y);
	
	if (get_tile_coord(window).is_in_bounds(map) == true &&
		map.at(get_tile_coord(window)).character_is_present())
	{
		auto charr = map.at(get_tile_coord(window)).get_character();
		auto text  = charr->get_stats().hp.to_text();
		
		text.draw_literal(window, 
			x + (TILE_WIDTH / 2)  - (text.get_w() / 2),
			y - (TILE_HEIGHT / 2) - (text.get_h() / 2));
	}
}

bool Cursor::update(const std::unique_ptr<SDL_Event> &event)
{
	if (event != nullptr && event->type == SDL_MOUSEMOTION &&
	   (event->motion.x - event->motion.x % TILE_WIDTH != x ||
	    event->motion.y - event->motion.y % TILE_HEIGHT != y))
	{
		x = event->motion.x - event->motion.x % TILE_WIDTH;
		y = event->motion.y - event->motion.y % TILE_HEIGHT;
		return true;
	}
	
	return false;
}
