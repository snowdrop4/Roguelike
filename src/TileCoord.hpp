#pragma once

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
