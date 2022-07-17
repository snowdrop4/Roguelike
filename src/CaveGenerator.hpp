#pragma once

#include <vector>
#include "AStar.hpp"

enum class CellType { floor, rock };

class CaveGenerator
{
	public:
		CaveGenerator(size_t width, size_t height);
		~CaveGenerator();
		
		size_t get_w() const;
		size_t get_h() const;
		
		CellType get_type(size_t col, size_t row) const;
		size_t get_floor_neighbors(size_t col, size_t row) const;
		size_t get_rock_neighbors (size_t col, size_t row) const;
		
		void display() const;
		
		void resize(size_t width, size_t height);
		
		/// A density between 45 and 55 is optimal, the density required for
		/// good output (not one huge cavern) is higher for larger maps.
		///
		/// Maps with lots of small caverns (maps with a high density to size
		/// ratio) take a long time to generate because of the huge strain
		/// on the algorithm to make sure all the small caverns are connected
		///
		/// The ideal range for smoothing is 2-4
		/// (smoothing >4 doesn't do anything and slows things down)
		void generate(size_t density = 48, size_t smoothing = 4);
		
		struct Cell;
		
	private:
		void generation();
		void merge();
		
		size_t width, height;
		
		std::vector<std::vector<Cell>> cells;
		AStar<Cell> astar;
};
