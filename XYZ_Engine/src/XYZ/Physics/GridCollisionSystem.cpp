#include "stdafx.h"
#include "GridCollisionSystem.h"

#define OUT_OF_GRID (1 << 31)
namespace XYZ {
	GridCollisionSystem::GridCollisionSystem()
		: m_NumRows(1), m_NumCols(1), m_CellSize(1), m_PositionX(0), m_PositionY(0)
	{
		m_Signature.set(XYZ::ECSManager::Get().GetComponentType<GridBody>());
		m_Signature.set(XYZ::ECSManager::Get().GetComponentType<CollisionComponent>());

		m_GridBodyStorage = ECSManager::Get().GetComponentStorage<GridBody>();
		m_CollisionStorage = ECSManager::Get().GetComponentStorage<CollisionComponent>();
	}
	void GridCollisionSystem::ClearGrid()
	{
		for (int i = 0; i < m_NumRows; ++i)
			for (int j = 0; j < m_NumCols; ++j)
				m_Cells[i][j].Mask = 0;
	}
	void GridCollisionSystem::ResizeGrid(int numRows, int numCols, int cellSize, int positionX, int positionY)
	{
		m_NumRows = numRows;
		m_NumCols = numCols;
		m_CellSize = cellSize; 
		m_PositionX = positionX; 
		m_PositionY = positionY;

		m_Cells.resize(m_NumRows);
		for (int i = 0; i < m_NumRows; ++i)
			m_Cells[i].resize(m_NumCols);
	}
	void GridCollisionSystem::MoveGrid(int positionX, int positionY)
	{
		m_PositionX = positionX;
		m_PositionY = positionY;
	}
	void GridCollisionSystem::Update(float dt)
	{
		for (auto& it : m_Components)
		{
			if (((*m_ActiveStorage)[it.ActiveIndex].ActiveComponents & m_Signature) == m_Signature)
			{
				auto mask = (*m_CollisionStorage)[it.CollisionIndex].Layer;
				int32_t result = Move(
					(*m_GridBodyStorage)[it.GridBodyIndex],
					(*m_CollisionStorage)[it.CollisionIndex].CollisionLayers,
					(*m_CollisionStorage)[it.CollisionIndex].Layer);

				// Store mask of layer it collides with
				(*m_CollisionStorage)[it.CollisionIndex].CurrentCollisions = result;
				
				// No collisions , free to move
				if (!result && result != OUT_OF_GRID)
				{
					(*m_GridBodyStorage)[it.GridBodyIndex].Col += (*m_GridBodyStorage)[it.GridBodyIndex].NextCol;
					(*m_GridBodyStorage)[it.GridBodyIndex].Row += (*m_GridBodyStorage)[it.GridBodyIndex].NextRow;			
				}
				else
				{
					//(*m_GridBodyStorage)[it.gridBodyIndex].nextCol = 0;
					//(*m_GridBodyStorage)[it.gridBodyIndex].nextRow = 0;
				}		
			}
		}

	}
	void GridCollisionSystem::Add(Entity entity)
	{
		Component component;
		component.Ent = entity;
		component.CollisionIndex = ECSManager::Get().GetComponentIndex<CollisionComponent>(entity);
		component.GridBodyIndex = ECSManager::Get().GetComponentIndex<GridBody>(entity);
		component.ActiveIndex = ECSManager::Get().GetComponentIndex<ActiveComponent>(entity);

		auto layer = (*m_CollisionStorage)[component.CollisionIndex].Layer;
		auto collisionLayer = (*m_CollisionStorage)[component.CollisionIndex].CollisionLayers;
		if (Insert((*m_GridBodyStorage)[component.GridBodyIndex],collisionLayer, layer))
		{
			m_Components.push_back(component);
			XYZ_LOG_INFO("Entity with ID ", entity, " added");
		}
		else
			XYZ_LOG_WARN("Entity with ID ", entity, " not added");
	}
	void GridCollisionSystem::Remove(Entity entity)
	{
		auto it = std::find(m_Components.begin(), m_Components.end(), entity);
		if (it != m_Components.end())
		{
			XYZ_LOG_INFO("Entity with id ", entity, " removed");
			auto mask = (*m_CollisionStorage)[it->CollisionIndex].Layer;
			Remove((*m_GridBodyStorage)[it->GridBodyIndex], mask);
			*it = std::move(m_Components.back());
			m_Components.pop_back();
		}
	}
	bool GridCollisionSystem::Contains(Entity entity)
	{
		auto it = std::find(m_Components.begin(), m_Components.end(), entity);
		if (it != m_Components.end())
			return true;
		return false;
	}
	bool GridCollisionSystem::Insert(const GridBody& body, int32_t collisionMask, int32_t layerMask)
	{
		if (body.Row + body.Height < m_NumRows && body.Col + body.Width < m_NumCols
			&& body.Row >= m_PositionY && body.Col >= m_PositionX)
		{
			for (int i = body.Row; i < body.Row + body.Height; ++i)
				for (int j = body.Col; j < body.Col + body.Width; ++j)
					if (m_Cells[i][j].Mask & collisionMask)
						return false;

			for (int i = body.Row; i < body.Row + body.Height; ++i)
				for (int j = body.Col; j < body.Col + body.Width; ++j)
					m_Cells[i][j].Mask |= layerMask;


			return true;
		}
		return false;
	}
	int32_t GridCollisionSystem::Move(const GridBody& oldBody,int32_t collisionMask, int32_t layerMask)
	{
		GridBody newBody = oldBody;
		newBody.Col += newBody.NextCol;
		newBody.Row += newBody.NextRow;

		if (newBody.Row + newBody.Height < m_NumRows && newBody.Col + newBody.Width < m_NumCols
			&& newBody.Row >= m_PositionY && newBody.Col >= m_PositionX)
		{
			// No changes to the body
			if (oldBody == newBody)
				return 0;

			if (newBody.Col < oldBody.Col)
			{
				// Handle diagonal movement
				if (newBody.Row != oldBody.Row)
				{
					if (m_Cells[oldBody.Row][oldBody.Col - 1].Mask & collisionMask)
					{
						return m_Cells[oldBody.Row][oldBody.Col - 1].Mask & collisionMask;
					}
					else if (m_Cells[newBody.Row][newBody.Col + newBody.Width].Mask & collisionMask)
					{
						return m_Cells[newBody.Row][newBody.Col + newBody.Width].Mask & collisionMask;
					}
				}
				else
				{
					// Check all cells to the top of the body
					for (int i = newBody.Row; i < newBody.Row + newBody.Height; ++i)
						if (m_Cells[i][newBody.Col].Mask & collisionMask)
							return m_Cells[i][newBody.Col].Mask & collisionMask;
				}
			}
			else if (newBody.Col > oldBody.Col)
			{
				// Handle diagonal movement
				if (newBody.Row != oldBody.Row)
				{
					if (m_Cells[oldBody.Row][oldBody.Col + oldBody.Width].Mask & collisionMask)
					{
						return m_Cells[oldBody.Row][oldBody.Col + oldBody.Width].Mask & collisionMask;
					}
					else if (m_Cells[newBody.Row][newBody.Col - 1].Mask & collisionMask)
					{
						return m_Cells[newBody.Row][newBody.Col - 1].Mask & collisionMask;
					}
				}
				else
				{
					// Check all cells to the top of the body and one cell in the width of the new body
					for (int i = newBody.Row; i < newBody.Row + newBody.Height; ++i)
						if (m_Cells[i][newBody.Col + newBody.Width - 1].Mask & collisionMask)
							return m_Cells[i][newBody.Col + newBody.Width - 1].Mask & collisionMask;
				}
			}
			else if (newBody.Row < oldBody.Row)
			{
				// Check all cells to the width of the body
				for (int i = newBody.Col; i < newBody.Col + newBody.Width; ++i)
					if (m_Cells[newBody.Row][i].Mask & collisionMask)
						return m_Cells[newBody.Row][i].Mask & collisionMask;
			}
			else
			{
				// Check all cells to the width of the body and one to the top
				for (int i = newBody.Col; i < newBody.Col + newBody.Width; ++i)
					if (m_Cells[newBody.Row + newBody.Height - 1][i].Mask & collisionMask)
						return m_Cells[newBody.Row + newBody.Height - 1][i].Mask & collisionMask;
			}

			// Clear all the cells occupied by the oldBody
			for (int i = oldBody.Row; i < oldBody.Row + oldBody.Height; ++i)
				for (int j = oldBody.Col; j < oldBody.Col + oldBody.Width; ++j)
					m_Cells[i][j].Mask &= (~layerMask);

			// Set all the cells occupied by the newBody
			for (int i = newBody.Row; i < newBody.Row + newBody.Height; ++i)
				for (int j = newBody.Col; j < newBody.Col + newBody.Width; ++j)
					m_Cells[i][j].Mask |= layerMask;


			// Movement was successful return empty collision mask
			return 0;
		}
		return 1 << 31;
	}
	void GridCollisionSystem::Remove(const GridBody& body, int32_t layerMask)
	{
		if (body.Row + body.Height < m_NumRows && body.Col + body.Width < m_NumCols
			&& body.Row >= m_PositionY && body.Col >= m_PositionX)
		{
			// Clear all cells occupied by the body
			for (int i = body.Row; i < body.Row + body.Height; ++i)
				for (int j = body.Col; j < body.Col + body.Width; ++j)
					m_Cells[i][j].Mask &= (~layerMask);
		}
	}
}