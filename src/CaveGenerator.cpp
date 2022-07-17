#include "CaveGenerator.hpp"
#include <iostream>
#include <deque>

struct CaveGenerator::Cell
{
	CellType type;
	int cavernID;
};

CaveGenerator::CaveGenerator(size_t width, size_t height) :
	width(width),
	height(height),
	cells(),
	astar(cells, width, height)
{
	
}

CaveGenerator::~CaveGenerator() { }

size_t CaveGenerator::get_w() const
{ return width; }

size_t CaveGenerator::get_h() const
{ return height; }

CellType CaveGenerator::get_type(size_t col, size_t row) const
{ return cells[row][col].type; }

size_t CaveGenerator::get_floor_neighbors(size_t col, size_t row) const
{
	size_t ret = 0;
	
	for (int q = -1; q < 2; ++q)
	for (int w = -1; w < 2; ++w)
		if ((q != 0 || w != 0) &&
			col + w < width  && col + w > 0 &&
			row + q < height && row + q > 0 &&
			cells[row + q][col + w].type == CellType::floor)
			++ret;
	return ret;
}

size_t CaveGenerator::get_rock_neighbors(size_t col, size_t row) const
{
	size_t ret = 0;
	
	for (int q = -1; q < 2; ++q)
	for (int w = -1; w < 2; ++w)
		if ((q != 0 || w != 0) &&
			col + w < width  && col + w > 0 &&
			row + q < height && row + q > 0 &&
			cells[row + q][col + w].type == CellType::rock)
			++ret;
	return ret;
}

void CaveGenerator::display() const
{
	std::cout << '\t';
	for (size_t i = 0; i < width; ++i)
	{
		if (i % 10 == 0)
			std::cout << std::to_string(i)[0];
		else
			std::cout << " ";
	}
	
	std::cout << "\n\t";
	
	for (size_t i = 0; i < width; ++i)
		std::cout << std::to_string(i)[std::to_string(i).size() - 1];
	
	std::cout << '\n';
	
	for (size_t i = 0; i < height; ++i)
	{
		std::cout << i << '\t';
		for (size_t j = 0; j < width; ++j)
		{
			switch (cells[i][j].type)
			{
				case CellType::floor: std::cout << '.'; break;
				case CellType::rock:  std::cout << '#'; break;
			}
		}
		std::cout << '\n';
	}
}

void CaveGenerator::resize(size_t width_A, size_t height_A)
{
	width = width_A;
	height = height_A;
	astar.resize(width, height);
}

void CaveGenerator::generate(size_t density, size_t smoothing)
{
	cells.clear();
	
	cells.resize(height);
	for (auto &i : cells)
		i.resize(width);
	
	// creating the walls along the edge of the cells first, and then not ever
	// changing them, but including them as neighbors when running the
	// generations seems to create better results than creating the walls along
	// the edge of the cells when all the generations are done.
	for (size_t i = 0; i < width; ++i)
	{
		cells[0][i].type = CellType::rock;
		cells[height - 1][i].type = CellType::rock;
	}
	for (size_t i = 0; i < height; ++i)
	{
		cells[i][0].type = CellType::rock;
		cells[i][width - 1].type = CellType::rock;
	}
		
	for (size_t i = 1; i < height - 1; ++i)
	for (size_t j = 1; j < width  - 1; ++j)
	{
		if ((size_t)rand() % 100 < density)
			cells[i][j].type = CellType::rock;
		else
			cells[i][j].type = CellType::floor;
	}
	
	for (size_t i = 0; i < smoothing; ++i)
		generation();
	
	merge();
}

void CaveGenerator::generation()
{
	for (size_t i = 1; i < height - 1; ++i)
	for (size_t j = 1; j < width  - 1; ++j)
	{
		size_t rockNeighbors = 0;
		size_t floorNeighbors = 0;
		
		for (int q = -1; q < 2; ++q)
		for (int w = -1; w < 2; ++w)
		{
			if (q != 0 || w != 0)
				switch (cells[i + q][j + w].type)
				{
					case CellType::rock:  ++rockNeighbors;  break;
					case CellType::floor: ++floorNeighbors; break;
				}
		}
		
		// invalidates the neighbor data in the surrounding cells
		if (rockNeighbors >= 5)
			cells[i][j].type = CellType::rock;
		else if (rockNeighbors <= 3)
			cells[i][j].type = CellType::floor;
	}
}

static std::unique_ptr<std::deque<std::pair<size_t, size_t>>>
flood_fill_w_cavern_id(size_t startX, size_t startY, int cavernID,
	std::vector<std::vector<CaveGenerator::Cell>> &map);

/// detect isolated chunks for floor tiles, and dig a path between them
void CaveGenerator::merge()
{
	for (size_t i = 0; i < height; ++i)
	for (size_t j = 0; j < width; ++j)
		cells[i][j].cavernID = -1;
	
	// deque containing every floor tile grouped by the cavern it is in
	std::deque<std::unique_ptr<
		std::deque<std::pair<size_t, size_t>>
	>> caverns;
	
	size_t numCaverns = -1;
	
	for (size_t i = 1; i < height - 1; ++i)
	for (size_t j = 1; j < width  - 1; ++j)
	{
		if (cells[i][j].type == CellType::floor && cells[i][j].cavernID == -1)
		{
			auto s = flood_fill_w_cavern_id(j, i, ++numCaverns, cells);
			caverns.emplace_back(s.release());
		}
	}
	
	auto valid_tile =
	[](const CaveGenerator::Cell &cell, size_t xDiff, size_t yDiff) -> bool
	{
		if (xDiff != 0 && yDiff != 0)
			return false;
		
		return true;
	};
	
	auto move_cost =
	[](const CaveGenerator::Cell &cell, size_t xDiff, size_t yDiff) -> size_t
	{
		if (cell.type == CellType::rock)
			return 30;
		return 10;
	};
	
	for (size_t i = 0; i < numCaverns; ++i)
	{
		auto start  = (*caverns[i    ])[rand() % caverns[i    ]->size()];
		auto finish = (*caverns[i + 1])[rand() % caverns[i + 1]->size()];
		
		auto path =	astar.get_path(
			start.first, start.second, finish.first, finish.second,
			valid_tile, move_cost, AStarFuncs::dist_cost);
		
		for (auto &i : *path)
			cells[i.second][i.first].type = CellType::floor;
	}
}

static std::unique_ptr<std::deque<std::pair<size_t, size_t>>>
flood_fill_w_cavern_id(size_t startX, size_t startY, int cavernID,
	std::vector<std::vector<CaveGenerator::Cell>> &map)
{
	std::unique_ptr<std::deque<std::pair<size_t, size_t>>> tilesInCavern
		(new std::deque<std::pair<size_t, size_t>>);
	
	std::deque<std::pair<size_t, size_t>> openSet;
	std::deque<std::pair<size_t, size_t>> nextOpenSet;
	
	auto add_to_if_valid =
	[&](size_t x, size_t y, std::deque<std::pair<size_t, size_t>> &set)
	{
		if (map[y][x].type == CellType::floor && map[y][x].cavernID == -1)
		{
			set.emplace_back(x, y);
			tilesInCavern->emplace_back(x, y);
			map[y][x].cavernID = cavernID;
		}
	};
	
	add_to_if_valid(startX, startY, openSet);
	add_to_if_valid(startX - 1, startY, openSet);
	add_to_if_valid(startX + 1, startY, openSet);
	add_to_if_valid(startX, startY - 1, openSet);
	add_to_if_valid(startX, startY + 1, openSet);
	
	while (openSet.size() != 0)
	{
		for (auto &i : openSet)
		{
			add_to_if_valid(i.first - 1, i.second, nextOpenSet);
			add_to_if_valid(i.first + 1, i.second, nextOpenSet);
			add_to_if_valid(i.first, i.second - 1, nextOpenSet);
			add_to_if_valid(i.first, i.second + 1, nextOpenSet);
		}
		
		openSet.clear();
		std::swap(openSet, nextOpenSet);
	}
	
	return tilesInCavern;
}
