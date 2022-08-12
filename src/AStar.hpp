#pragma once

#include <vector>
#include <list>
#include <memory>
#include <map>
#include <cmath>
#include <functional>

// Type synonyms for the arguments to AStar::get_path(),
// as the type definition gets unweildy
template<typename T> using CellIsValid = std::function<bool (const T &, int xDiff, int yDiff)>;
template<typename T> using MoveCost    = std::function<int  (const T &, int xDiff, int yDiff)>;
using DistCost = std::function<int  (int x1, int y1, int x2, int y2)>;

// Functions used as default arguments to AStar::get_path().
// Most of the time these should do the job, without needing to be changed.
namespace AStarFuncs
{
	const int STRAIGHT_COST = 10;
	const int DIAG_COST = 14;
	
	// Calculates the cost to move to a neighbouring cell, based on whether one
	// has to move straight or diagonally to get to it from the current cell.
	template<typename T>
	int move_cost(const T &cell, int xDiff, int yDiff)
	{
		if (xDiff != 0 && yDiff != 0)
			return DIAG_COST;
		
		return STRAIGHT_COST;
	}
	
	// Estimates the total cost to move from a cell to any another cell.
	// This estimate is based on straight movement only (no diagonals),
	// and assumes that there are no obstacles.
	//
	// This function can therefore overestimate or underestimate the distance
	// required, which means that AStar might not find the optimal path in all
	// cases (will only be slightly off when it doesn't).
	auto dist_cost = [](int x1, int y1, int x2, int y2) -> int
	{
		return std::abs(x1 - x2) + std::abs(y1 - y2) * STRAIGHT_COST;
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
			CellIsValid<T> cell_is_valid,
			MoveCost<T>    move_cost = AStarFuncs::move_cost<T>,
			DistCost       dist_cost = AStarFuncs::dist_cost) const;
		
		void resize(int width, int height);
		
	private:
		enum class BelongsTo { closedSet, openSet };
		
		struct node
		{
			node(int x, int y, int gCost, int hCost, BelongsTo belongsTo,
					std::shared_ptr<node> parent) :
				x(x), y(y),
				gCost(gCost),
				hCost(hCost),
				belongsTo(belongsTo),
				parent(parent)
			{
				
			}
			
			int x, y;
			
			// The cost needed to reach this cell from the start cell.
			int gCost;
			
			// An estimation of the cost required to reach the end
			// cell from this cell, without using diagonal movement, and
			// assuming there are no barriers in the way.
			int hCost;
			
			// Performance optimisation: adding a member to the node to keep
			// track of which set it is in saves us from having to lookup
			// this node inside our openSet every time we want to find out 
			// which set it's in.
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
	CellIsValid<T> cell_is_valid,
	MoveCost<T>    move_cost,
	DistCost       dist_cost) const
{
	// return value
	std::unique_ptr<std::list<std::pair<int, int>>>
		result(new std::list<std::pair<int, int>>{});
	
	// if the start cell and end cell are the same,
	// or if the end cell is invalid, we can just return
	if ((startX == endX && startY == endY) || cell_is_valid(map[endY][endX], 0, 0) == false)
		return result;
	
	// store nodes in the open set, ordered by the total cost (gCost + hCost)
	std::multimap<int, std::shared_ptr<node>> openSet;
	
	// add the start node to openSet and nodeMap
	int startHCost = dist_cost(startX, startY, endX, endY);
	
	auto startNode = std::make_shared<node>(startX, startY,
		0, startHCost, BelongsTo::openSet, nullptr);
	
	openSet.insert(std::make_pair(startHCost, startNode));
	nodeMap[startY][startX] = startNode;
	
	while (openSet.size() != 0)
	{
		// Find the entry with the lowest total cost (gCost + hCost) in
		// our open set.
		auto lowestTotalCost = openSet.lower_bound(0);
		auto parentNode = lowestTotalCost->second;
		
		// If that cell is the end cell, we're done!
		if (parentNode->x == endX && parentNode->y == endY)
		{
			for (auto curr = parentNode; curr != nullptr; curr = curr->parent)
				result->emplace_front(curr->x, curr->y);
			break;
		}
		// Else, move that cell to the closed set. It is this cell who's
		// neighbors we will next add to the open set, since its low cost means
		// that the shortest path is likely to run through this cell.
		else
		{
			parentNode->belongsTo = BelongsTo::closedSet;
			openSet.erase(lowestTotalCost);
		}
		
		static const std::pair<int, int> neighbors[8] = {
			{ -1, -1 }, {  0, -1 }, { +1, -1 },
			{ -1,  0 },             { +1,  0 },
			{ -1, +1 }, {  0, +1 }, { +1, +1 },
		};
		
		// for each cell adjacent to the parent cell:
		for (auto [w, q] : neighbors)
		{
			int newX = parentNode->x + w;
			int newY = parentNode->y + q;
			
			// skip if we're not in bounds or the cell is invalid:
			if (newX < 0 || newX >= width  ||
			    newY < 0 || newY >= height ||
			    cell_is_valid(map[newY][newX], w, q) == false)
			{
				continue;
			}
			
			// if there's no valid entry in nodeMap for the cell, the cell
			// cannot be in either the openSet or the closedSet; so add
			// a new node to the open set for the cell:
			if (nodeMap[newY][newX].expired() == true)
			{
				int gCost = move_cost(map[newY][newX], w, q) + parentNode->gCost;
				int hCost = dist_cost(newX, newY, endX, endY);
				
				auto newNode = std::make_shared<node>(newX, newY,
					gCost, hCost, BelongsTo::openSet, parentNode);
				
				openSet.insert(std::make_pair(gCost + hCost, newNode));
				nodeMap[newY][newX] = newNode;
			}
			// otherwise (as long as the node is in the open set) check to
			// see if the total cost of getting to the cell is lower if we
			// go through the parent cell
			else
			{
				int gCost = move_cost(map[newY][newX], w, q) + parentNode->gCost;
				int hCost = dist_cost(newX, newY, endX, endY);
				
				auto node = nodeMap[newY][newX].lock();
				
				if (node->belongsTo == BelongsTo::openSet &&
					gCost + hCost < node->gCost + node->hCost)
				{
					node->gCost = gCost;
					node->hCost = hCost;
					node->parent = parentNode;
				}
			}
		}
	}
	
	return result;
}
