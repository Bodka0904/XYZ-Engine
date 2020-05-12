#include "stdafx.h"
#include "HashGrid2D.h"


namespace XYZ {
	HashGrid2D::HashGrid2D(int cellSize, int width, int height)
		:
		m_CellSize(cellSize),
		m_Width(width),
		m_Height(height),
		m_TableSize(width* height)
	{
		m_Table.resize(m_TableSize);
	}
	void HashGrid2D::Insert(int element, const glm::vec2& pos, const glm::vec2& size)
	{
		for (int i = pos.x; i < pos.x + size.x; ++i)
		{
			for (int j = pos.y; j < pos.y + size.y; ++j)
			{
				int x = (int)floor(i / m_CellSize) % m_Width;
				int y = (int)floor(j / m_CellSize) % m_Height;

				int index = ((y * m_Width) + x);
				m_Table[(size_t)index].elements.push_back(element);
			}
		}
	}
	void HashGrid2D::Remove(int element, const glm::vec2& pos, const glm::vec2& size)
	{
		for (int i = pos.x; i < pos.x + size.x; ++i)
		{
			for (int j = pos.y; j < pos.y + size.y; ++j)
			{
				int x = (int)floor(i / m_CellSize) % m_Width;
				int y = (int)floor(j / m_CellSize) % m_Height;

				int index = ((y * m_Width) + x);
				auto it = std::find(m_Table[(size_t)index].elements.begin(), m_Table[(size_t)index].elements.end(), element);
				if (it != m_Table[index].elements.end())
					m_Table[index].elements.erase(it);
			}
		}
	}
	void HashGrid2D::Clear()
	{
		m_Table.clear();
	}
	void HashGrid2D::ClearCells()
	{
		for (auto& cell : m_Table)
			cell.elements.clear();
	}
	size_t HashGrid2D::GetElements(int** buffer, const glm::vec2& pos, const glm::vec2& size)
	{
		std::vector<size_t> indices;
		size_t elemSize = 0;
		for (int i = pos.x; i < pos.x + size.x; ++i)
		{
			for (int j = pos.y; j < pos.y + size.y; ++j)
			{
				size_t index = ((size_t)floor(i / m_CellSize) + (size_t)floor(j / m_CellSize)) % m_TableSize;
				elemSize += m_Table[index].elements.size();
				indices.push_back(index);
			}
		}

		*buffer = new int[elemSize * sizeof(int)];
		int* ptr = *buffer;
		for (auto it : indices)
		{
			size_t elementsSize = m_Table[it].elements.size();
			memcpy(ptr, m_Table[it].elements.data(), elementsSize * sizeof(int));
			ptr += elementsSize;
		}

		return elemSize;
	}

}