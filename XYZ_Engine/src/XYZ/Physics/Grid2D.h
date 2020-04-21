#pragma once
#include "Components.h"

namespace XYZ {

	template<typename T>
	using vector2D = std::vector<std::vector<T>>;


	class Grid2D
	{
	public:
		enum class MoveResult
		{
			None,
			Success,
			Collision,
			OutOfGrid
		};

		Grid2D(int numRows, int numCols, int cellSize);
		
		bool Insert(const GridPosition& pos, int32_t layerMask = 1);
		MoveResult Move(const GridPosition& oldPos, const GridPosition& newPos, int32_t layerMask = 1);
		void Remove(const GridPosition& pos, int32_t layerMask = 1);

		glm::vec2 GridToWorld(const GridPosition& pos);

		
	private:
		struct Cell
		{
			int32_t mask = 0;
		};


		int m_NumRows;
		int m_NumCols;
		int m_CellSize;

		vector2D<Cell> m_Cells;
	};
}