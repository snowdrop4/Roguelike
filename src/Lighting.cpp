#include "Lighting.hpp"
#include "utildefines.hpp"
#include "MapTile.hpp"

// http://roguebasin.roguelikedevelopment.org/index.php?title=FOV_using_recursive_shadowcasting
int multipliers[4][8] =
{
	{1,  0,  0, -1, -1,  0,  0,  1},
	{0,  1, -1,  0,  0, -1,  1,  0},
	{0,  1,  1,  0,  0, -1, -1,  0},
	{1,  0,  0,  1, -1,  0,  0, -1}
};

void cast_quadrant(const Map &map, std::function<void (TileCoord xy)> &callback, 
	TileCoord origin, int radius, int row,
	float start_slope, float end_slope,
	int xx, int xy, int yx, int yy)
{
	if (start_slope < end_slope)
		return;
	
	float next_start_slope = start_slope;
	for (int i = row; i <= radius; i++)
	{
		bool blocked = false;
		for (int dx = -i, dy = -i; dx <= 0; dx++)
		{
			
			float l_slope = (dx - 0.5) / (dy + 0.5);
			float r_slope = (dx + 0.5) / (dy - 0.5);
			
			if (start_slope < r_slope)
				continue;
			else if (end_slope > l_slope)
				break;
			
			int sax = dx * xx + dy * xy;
			int say = dx * yx + dy * yy;
			
			if ((sax < 0 && std::abs(sax) > origin.x) || (say < 0 && std::abs(say) > origin.y))
				continue;
			
			int ax = origin.x + sax;
			int ay = origin.y + say;
			
			if (ax >= map.get_tile_w() || ay >= map.get_tile_h())
				continue;
			
			int radius2 = radius * radius;
			if ((dx * dx + dy * dy) < radius2)
				callback( {ax, ay} );
			
			if (blocked)
			{
				if (map.at( {ax, ay} ).is_solid())
				{
					next_start_slope = r_slope;
					continue;
				}
				else
				{
					blocked = false;
					start_slope = next_start_slope;
				}
			}
			else if (map.at( {ax, ay} ).is_solid())
			{
				blocked = true;
				next_start_slope = r_slope;
				cast_quadrant(map, callback, origin, radius, i + 1, start_slope, l_slope, xx, xy, yx, yy);
			}
		}
		if (blocked)
			break;
	}
}

void for_visible_tiles(const Map &map, std::function<void (TileCoord xy)> callback,
	TileCoord origin, int radius)
{
	callback(origin);
	
	for (int i = 0; i < 8; i++)
		cast_quadrant(map, callback, origin, radius, 1, 1.0, 0.0,
			multipliers[0][i], multipliers[1][i], multipliers[2][i], multipliers[3][i]);
}

void cast_light(const Map &map, TileCoord origin, int intensity)
{
	for_visible_tiles(map,
		[&](TileCoord xy)
		{
			int lightLevel = MapTile::ILLUMINATION_MAX - origin.distance_from(xy) * (MapTile::ILLUMINATION_MAX / intensity);
			lightLevel = std::max(MapTile::ILLUMINATION_MIN, lightLevel);
			
			map.at(xy).set_illumination_level(lightLevel);
		},
		origin, intensity);
}
