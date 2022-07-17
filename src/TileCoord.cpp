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

#include "TileCoord.hpp"

#include <cmath>
#include "Map.hpp"
#include "constants.hpp"

// -----------------------------------------------------------------------------
// TileCoordDiff
// -----------------------------------------------------------------------------
TileCoordDiff::TileCoordDiff(const TileCoord &fir, const TileCoord &sec) :
	xDiff(0),
	yDiff(0)
{
	reset(fir, sec);
}

void TileCoordDiff::reset(const TileCoord &fir, const TileCoord &sec)
{
	xDiff = fir.x - sec.x;
	yDiff = fir.y - sec.y;
}

// -----------------------------------------------------------------------------
// TileCoord
// -----------------------------------------------------------------------------
TileCoord TileCoord::operator + (const TileCoordDiff &other) const
{ return TileCoord(*this) += other; }

TileCoord TileCoord::operator + (const TileCoord &other) const
{ return TileCoord(*this) += other; }

TileCoord TileCoord::operator - (const TileCoord &other) const
{ return TileCoord(*this) -= other; }

TileCoord TileCoord::operator + (int a) const
{ return TileCoord(*this) += a; }

TileCoord TileCoord::operator - (int a) const
{ return TileCoord(*this) -= a; }

TileCoord &TileCoord::operator += (const TileCoordDiff &other)
{
	x += other.xDiff;
	y += other.yDiff;
	return *this;
}

TileCoord &TileCoord::operator += (const TileCoord &other)
{
	x += other.x;
	y += other.y;
	return *this;
}

TileCoord &TileCoord::operator -= (const TileCoord &other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}

TileCoord &TileCoord::operator += (int a)
{
	x += a;
	y += a;
	return *this;
}

TileCoord &TileCoord::operator -= (int a)
{ return *this += (-a); }

bool TileCoord::operator == (const TileCoord &other) const
{ return x == other.x && y == other.y; }

bool TileCoord::operator != (const TileCoord &other) const
{ return !(*this == other); }

int TileCoord::distance_from(const TileCoord &other) const
{
	return sqrt(pow(std::abs(x - other.x), 2) + pow(std::abs(y - other.y), 2));	
}

bool TileCoord::is_in_bounds(const Map &map) const
{
	return (x >= 0 &&
	        y >= 0 &&
	        x < map.get_tile_w() &&
	        y < map.get_tile_h());
}

int TileCoord::get_x_pixels() const
{ return x * TILE_WIDTH; }

int TileCoord::get_y_pixels() const
{ return y * TILE_HEIGHT; }

std::string TileCoord::to_string() const
{ return "(" + std::to_string(x) + ", " + std::to_string(y) + ")"; }

// -----------------------------------------------------------------------------
// Util
// -----------------------------------------------------------------------------
std::ostream& operator<< (std::ostream &out, const TileCoord &a)
{
	out << "(" << a.x << ", " << a.y << ")";
	return out;
}

std::ostream& operator<< (std::ostream &out, const TileCoordDiff &a)
{
	out << "(" << a.xDiff << ", " << a.yDiff << ")";
	return out;
}
