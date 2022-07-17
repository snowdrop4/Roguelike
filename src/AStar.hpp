#pragma once

#include <vector>
#include <list>
#include <memory>
#include <map>
#include <cmath>
#include <functional>

#define CellIsValid std::function<bool (const T &, int xDiff, int yDiff)>
#define MoveCost    std::function<int  (const T &, int xDiff, int yDiff)>
#define DistCost    std::function<int  (int x1, int y1, int x2, int y2)>

/// Default functions to pass to AStar::get_path(...) -- most of the time these
/// should do the job fine.
namespace AStarFuncs
{
	const int HORIZ_VERT_COST = 10;
	const int DIAG_COST = 14;
	
	template<typename T>
	int move_cost(const T &cell, int xDiff, int yDiff)
	{
		if (xDiff != 0 && yDiff != 0)
			return DIAG_COST;
		
		return HORIZ_VERT_COST;
	}
	
	/// can overestimate the distance, this means that AStar might not find the
	/// optimal path in all cases (will only be slightly off when it doesn't)
	auto dist_cost = [](int x1, int y1, int x2, int y2) -> int
	{
		return std::abs(x1 - x2) + std::abs(y1 - y2) * HORIZ_VERT_COST;
	};
}

template <typename T>
class AStar
{
	public:
		AStar(const std::vector<std::vector<T>> &map, int width, int height);
		~AStar();
		
		std::unique_ptr<std::list<std::pair<int, int>>>
		get_path(int startX, int startY, int endX, int endY,
			CellIsValid cell_is_valid,
			MoveCost move_cost = AStarFuncs::move_cost<T>,
			DistCost dist_cost = AStarFuncs::dist_cost) const;
		
		void resize(int width, int height);
		
	private:
		enum class BelongsTo { closedSet, openSet };
		
		struct node
		{
			node(int x, int y, int moveCost, int distCost, BelongsTo belongsTo,
					std::shared_ptr<node> parent) :
				x(x), y(y),
				moveCost(moveCost),
				distCost(distCost),
				belongsTo(belongsTo),
				parent(parent)
			{
				
			}
			
			int x, y;
			
			// the minimum cost needed to reach this tile -- this is the sum 
			// the moveCost of the parent tile, and whether this tile was 
			// reached diagonally or not
			int moveCost;
			
			// an estimation of the total moveCost needed to get to the 
			// finish, without using diagonal movement, and assuming all the 
			// tiles are walkable
			int distCost;
			
			BelongsTo belongsTo;
			
			std::shared_ptr<node> parent;
		};
		
		int width, height;
		
		const std::vector<std::vector<T>> &map;
		mutable std::vector<std::vector<std::weak_ptr<node>>> nodeMap;
};

template <typename T>
AStar<T>::AStar(const std::vector<std::vector<T>> &map, int width, int height) :
	width(width),
	height(height),
	map(map),
	nodeMap(height)
{
	for (auto &i : nodeMap)
		i.resize(width);
}

template <typename T>
AStar<T>::~AStar() { }

template <typename T>
void AStar<T>::resize(int width_A, int height_A)
{
	width = width_A;
	height = height_A;
	nodeMap.resize(height);
	for (auto &i : nodeMap)
		i.resize(width);
}

template <typename T>
std::unique_ptr<std::list<std::pair<int, int>>>
AStar<T>::get_path(int startX, int startY, int endX, int endY,
	CellIsValid cell_is_valid,
	MoveCost    move_cost,
	DistCost    dist_cost) const
{
	std::unique_ptr<std::list<std::pair<int, int>>> result(new std::list<std::pair<int, int>>{}); // return value
	
	if ((startX == endX && startY == endY) || cell_is_valid(map[endY][endX], 0, 0) == false)
		return result;
	
	std::multimap<int, std::shared_ptr<node>> openSet;
	
	// add the start node to openSet and nodeMap
	std::shared_ptr<node> startNode(new node(startX, startY, 0,
		dist_cost(startX, startY, endX, endY), BelongsTo::openSet, nullptr));
	
	openSet.insert(std::make_pair(dist_cost(startX, startY, endX, endY), startNode));
	nodeMap[startY][startX] = startNode;
	
	while (openSet.size() != 0)
	{
		// move the cell with the lowest total cost to the closed set.
		auto bestCell = openSet.lower_bound(0);
		auto parentCell = bestCell->second;
		parentCell->belongsTo = BelongsTo::closedSet;
		openSet.erase(bestCell);
		
		int x = parentCell->x;
		int y = parentCell->y;
		
		// if the end cell was just added to the closed set, we're done!
		if (x == endX && y == endY)
		{
			for (auto curr = parentCell; curr != nullptr; curr = curr->parent)
				result->emplace_front(curr->x, curr->y);
			break;
		}
		
		// for each cell adjacent to the parent cell:
		for (int q = -1; q < 2; ++q)
		for (int w = -1; w < 2; ++w)
		{
			int newX = x + w;
			int newY = y + q;
			// if the cell is in bounds, valid, and not on the closedSet
			if (newX >= 0 && newX < width  &&
			    newY >= 0 && newY < height &&
				cell_is_valid(map[newY][newX], w, q) &&
				(nodeMap[newY][newX].expired() == true || 
				 nodeMap[newY][newX].lock()->belongsTo != BelongsTo::closedSet))
			{
				int distCost = dist_cost(newX, newY, endX, endY);
				
				int moveCost = move_cost(map[newY][newX], w, q) +
				               parentCell->moveCost;
				
				auto cell = nodeMap[newY][newX].lock();
				
				// if the cell is not in the openSet, add it to the openSet
				if (nodeMap[newY][newX].expired() == true)
				{
					std::shared_ptr<node> newNode(new node(newX, newY,
						moveCost, distCost, BelongsTo::openSet, parentCell));
					
					openSet.insert(std::make_pair(moveCost + distCost, newNode));
					nodeMap[newY][newX] = newNode;
				}
				// Otherwise, check to see if the total cost of getting to the
				// cell is lower if we go through the parent cell
				else if (moveCost + distCost < cell->moveCost + cell->distCost)
				{
					cell->moveCost = moveCost;
					cell->distCost = distCost;
					cell->parent = parentCell;
				}
			}
		}
	}
	
	return result;
}
