#include "stdafx.h"
#include "Grid2D.h"


namespace XYZ {
	Grid2D::Grid2D(int numRows, int numCols, int cellSize)
		: m_NumRows(numRows),m_NumCols(numCols),m_CellSize(cellSize)
	{
		m_Cells.resize(m_NumRows);
		for (int i = 0; i < m_NumRows; ++i)
			m_Cells[i].resize(m_NumCols);
	}

	bool Grid2D::Insert(const GridPosition& pos, int32_t layerMask)
	{
		if (pos.row < m_NumRows && pos.col < m_NumCols &&
			pos.row >= 0 && pos.col >= 0)
		{
			if (m_Cells[pos.row][pos.col].mask & layerMask)
				return false;
			

			m_Cells[pos.row][pos.col].mask |= layerMask;
			return true;
		}
		return false;
	}

	Grid2D::MoveResult Grid2D::Move(const GridPosition& oldPos, const GridPosition& newPos, int32_t layerMask)
	{
		if (newPos.row < m_NumRows && newPos.col < m_NumCols && 
			newPos.row >= 0 && newPos.col >= 0)
		{	
			if (oldPos == newPos)
			{
				return Grid2D::MoveResult::None;
			}

			if (m_Cells[newPos.row][newPos.col].mask & layerMask)
			{
				return Grid2D::MoveResult::Collision;
			}
			else
			{
				m_Cells[oldPos.row][oldPos.col].mask &= (~layerMask);
				m_Cells[newPos.row][newPos.col].mask |= layerMask;
				return Grid2D::MoveResult::Success;
			}
		}
		return Grid2D::MoveResult::OutOfGrid;
	}
	void Grid2D::Remove(const GridPosition& pos, int32_t layerMask)
	{
		if (pos.row < m_NumRows && pos.col < m_NumCols &&
			pos.row >= 0 && pos.col >= 0)
		{		
			m_Cells[pos.row][pos.col].mask &= (~layerMask);	
		}
	}
	glm::vec2 Grid2D::GridToWorld(const GridPosition& pos)
	{
		return glm::vec2(pos.col * m_CellSize, pos.row * m_CellSize);
	}
}