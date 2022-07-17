#pragma once

/*
 * Copyright 2012 Alyxr
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with This program.  If not, see <http://www.gnu.org/licenses/>.
 * 
*/

#include <utility>
#include <ostream>

class Map;
struct TileCoord;

struct TileCoordDiff
{
	int xDiff, yDiff;
	
	TileCoordDiff(int x, int y) : xDiff(x), yDiff(y) { }
	TileCoordDiff(const TileCoord &, const TileCoord &);
	
	/// calculate a new diff
	void reset(const TileCoord &, const TileCoord &);
};

struct TileCoord
{
	int x, y;
	
	TileCoord(int x, int y) : x(x), y(y) { }
	TileCoord(const std::pair<int, int> &a) : x(a.first), y(a.second) { }
	
	TileCoord operator + (const TileCoordDiff &other) const;
	TileCoord operator + (const TileCoord     &other) const;
	TileCoord operator - (const TileCoord     &other) const;
	TileCoord operator + (int) const;
	TileCoord operator - (int) const;
	
	TileCoord &operator += (const TileCoordDiff &other);
	TileCoord &operator += (const TileCoord     &other);
	TileCoord &operator -= (const TileCoord     &other);
	TileCoord &operator += (int);
	TileCoord &operator -= (int);
	
	bool operator == (const TileCoord &other) const;
	bool operator != (const TileCoord &other) const;
	
	/// "as the crow flies" distance
	int distance_from(const TileCoord &other) const;
	
	bool is_in_bounds(const Map &) const;
	
	/// returns the x,y position in pixels
	int get_x_pixels() const;
	int get_y_pixels() const;
	
	std::string to_string() const;
};

std::ostream& operator<< (std::ostream &out, const TileCoord &);
std::ostream& operator<< (std::ostream &out, const TileCoordDiff &);
