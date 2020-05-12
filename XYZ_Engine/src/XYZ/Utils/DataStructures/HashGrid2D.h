#pragma once


#include <glm/glm.hpp>

namespace XYZ {
	class HashGrid2D
	{
	public:
		HashGrid2D(int cellSize, int width, int height);


		void Insert(int element, const glm::vec2& pos, const glm::vec2& size);
		void Remove(int element, const glm::vec2& pos, const glm::vec2& size);
		void Clear();
		void ClearCells();

		size_t GetElements(int** buffer, const glm::vec2& pos, const glm::vec2& size);

	private:
		struct Cell
		{
			std::vector<int> elements;
		};

		int m_CellSize;
		int m_Height;
		int m_Width;
		int m_TableSize;

		std::vector<Cell> m_Table;
	};
}