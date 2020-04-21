#include "stdafx.h"
#include "GridSystem2D.h"

namespace XYZ {
	GridSystem2D::GridSystem2D(int rows, int cols, int cellSize)
		:
		m_Rows(rows),
		m_Cols(cols),
		m_CellSize(cellSize)
	{
		m_Signature.set(XYZ::ECSManager::Get()->GetComponentType<XYZ::Transform2D>());
		m_Signature.set(XYZ::ECSManager::Get()->GetComponentType<XYZ::RigidBody2D>());

		m_Cells.resize(m_Rows);
		for (size_t i = 0; i < m_Rows; ++i)
			m_Cells[i].resize(m_Cols);
	}
}