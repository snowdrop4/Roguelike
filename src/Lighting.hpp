#include "Map.hpp"

void for_visible_tiles(const Map &map, std::function<void (TileCoord xy)> callback,
	TileCoord origin, int radius);

void cast_light(const Map &map, TileCoord origin, int intensity);
