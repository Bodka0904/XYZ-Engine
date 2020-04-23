#include "stdafx.h"
#include "PhysicsSystem.h"


#include "XYZ/Timer.h"

namespace XYZ {
	PhysicsSystem::PhysicsSystem()
	{
		m_Signature.set(XYZ::ECSManager::Get()->GetComponentType<XYZ::Renderable2D>());
		m_Signature.set(XYZ::ECSManager::Get()->GetComponentType<XYZ::RigidBody2D>());
		m_Signature.set(XYZ::ECSManager::Get()->GetComponentType<XYZ::GridPosition>());
		m_Signature.set(XYZ::ECSManager::Get()->GetComponentType<XYZ::InterpolatedMovement>());
		m_Signature.set(XYZ::ECSManager::Get()->GetComponentType<XYZ::LayerMask>());

		m_RenderableStorage = ECSManager::Get()->GetComponentStorage<Renderable2D>();
		m_BodyStorage = ECSManager::Get()->GetComponentStorage<RigidBody2D>();
		m_GridPosStorage = ECSManager::Get()->GetComponentStorage<GridPosition>();
		m_InterpolStorage = ECSManager::Get()->GetComponentStorage<InterpolatedMovement>();
		m_LayerStorage = ECSManager::Get()->GetComponentStorage<LayerMask>();

		m_Grid = nullptr;
	}
	PhysicsSystem::~PhysicsSystem()
	{
		delete m_Grid;
	}
	void PhysicsSystem::Update(float dt)
	{	
		for (auto &it : m_Components)
		{
			if (!(*m_InterpolStorage)[it.interpolIndex].inProgress)
			{
				auto mask = (*m_LayerStorage)[it.layerIndex].mask;
				auto result = m_Grid->Move((*m_GridPosStorage)[it.gridPosIndex], (*m_GridPosStorage)[it.gridPosIndex] + (*m_BodyStorage)[it.bodyIndex],mask);
				if (result == Grid2D::MoveResult::Success)
				{		
					(*m_InterpolStorage)[it.interpolIndex].inProgress = true;
					(*m_InterpolStorage)[it.interpolIndex].velocityX = (*m_BodyStorage)[it.bodyIndex].velocityX;
					(*m_InterpolStorage)[it.interpolIndex].velocityY = (*m_BodyStorage)[it.bodyIndex].velocityY;
				}
				else if (result == Grid2D::MoveResult::Collision)
				{
					(*m_BodyStorage)[it.bodyIndex].velocityX = 0;
					(*m_BodyStorage)[it.bodyIndex].velocityY = 0;
					(*m_InterpolStorage)[it.interpolIndex].currentTime = 0.0f;
					(*m_InterpolStorage)[it.interpolIndex].inProgress = false;
				}
				else if (result == Grid2D::MoveResult::OutOfGrid)
				{
					(*m_BodyStorage)[it.bodyIndex].velocityX = 0;
					(*m_BodyStorage)[it.bodyIndex].velocityY = 0;
					(*m_InterpolStorage)[it.interpolIndex].currentTime = 0.0f;
					(*m_InterpolStorage)[it.interpolIndex].inProgress = false;
				}
			}
			AnimateMovement(it, dt);	
		}

	}

	void PhysicsSystem::Add(Entity entity)
	{	
		XYZ_ASSERT(m_Grid, "Grid was not created");

		Component component;
		component.entity = entity;
		component.renderIndex = ECSManager::Get()->GetComponentIndex<Renderable2D>(entity);
		component.bodyIndex = ECSManager::Get()->GetComponentIndex<RigidBody2D>(entity);
		component.gridPosIndex = ECSManager::Get()->GetComponentIndex<GridPosition>(entity);
		component.interpolIndex = ECSManager::Get()->GetComponentIndex<InterpolatedMovement>(entity);
		component.layerIndex = ECSManager::Get()->GetComponentIndex<LayerMask>(entity);

		
		auto mask = (*m_LayerStorage)[component.layerIndex].mask;
		if (m_Grid->Insert((*m_GridPosStorage)[component.gridPosIndex], mask))
		{
			(*m_RenderableStorage)[component.renderIndex].position.x = (float)(*m_GridPosStorage)[component.gridPosIndex].col;
			(*m_RenderableStorage)[component.renderIndex].position.y = (float)(*m_GridPosStorage)[component.gridPosIndex].row;
			m_Components.push_back(component);
			XYZ_LOG_INFO("Entity with ID ", entity, " added");
		}
		else
		{
			(*m_GridPosStorage)[component.gridPosIndex].col = 0;
			(*m_GridPosStorage)[component.gridPosIndex].row = 0;
			XYZ_LOG_WARN("Failed to insert entity with ID ", entity, " to grid");
		}	
	}

	void PhysicsSystem::Remove(Entity entity)
	{
		auto it = std::find(m_Components.begin(), m_Components.end(), entity);
		if (it != m_Components.end())
		{
			XYZ_LOG_INFO("Entity with id ", entity, " removed");
			auto mask = (*m_LayerStorage)[it->layerIndex].mask;
			m_Grid->Remove((*m_GridPosStorage)[it->gridPosIndex],mask);
			*it = std::move(m_Components.back());
			m_Components.pop_back();
		}
	}

	bool PhysicsSystem::Contains(Entity entity)
	{
		auto it = std::find(m_Components.begin(), m_Components.end(), entity);
		if (it != m_Components.end())
			return true;
		return false;
	}

	void PhysicsSystem::CreateGrid(int numRows, int numCols, int cellSize)
	{
		if (m_Grid)
		{
			delete m_Grid;
			m_Grid = nullptr;
		}
		m_Grid = new Grid2D(numRows, numCols, cellSize);
	}

	void PhysicsSystem::AnimateMovement(const Component& it, float dt)
	{
		if ((*m_InterpolStorage)[it.interpolIndex].inProgress)
		{
			int x = (*m_InterpolStorage)[it.interpolIndex].velocityX;
			int y = (*m_InterpolStorage)[it.interpolIndex].velocityY;

			(*m_RenderableStorage)[it.renderIndex].position.x += x * (dt / (*m_InterpolStorage)[it.interpolIndex].length);
			(*m_RenderableStorage)[it.renderIndex].position.y += y * (dt / (*m_InterpolStorage)[it.interpolIndex].length);
			(*m_InterpolStorage)[it.interpolIndex].currentTime += dt;

			if ((*m_InterpolStorage)[it.interpolIndex].currentTime >= (*m_InterpolStorage)[it.interpolIndex].length)
			{
				(*m_InterpolStorage)[it.interpolIndex].inProgress = false;

				(*m_GridPosStorage)[it.gridPosIndex].col += (*m_InterpolStorage)[it.interpolIndex].velocityX;
				(*m_GridPosStorage)[it.gridPosIndex].row += (*m_InterpolStorage)[it.interpolIndex].velocityY;

				(*m_InterpolStorage)[it.interpolIndex].currentTime = 0.0f;
				(*m_InterpolStorage)[it.interpolIndex].velocityX = 0;
				(*m_InterpolStorage)[it.interpolIndex].velocityY = 0;
			}
		}
	}

}