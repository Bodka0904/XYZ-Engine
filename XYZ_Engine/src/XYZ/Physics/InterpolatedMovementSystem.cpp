#include "stdafx.h"
#include "InterpolatedMovementSystem.h"




namespace XYZ {
	InterpolatedMovementSystem::InterpolatedMovementSystem()
	{
		m_Signature.set(XYZ::ECSManager::Get()->GetComponentType<InterpolatedMovement>());
		m_Signature.set(XYZ::ECSManager::Get()->GetComponentType<Renderable2D>());
		m_Signature.set(XYZ::ECSManager::Get()->GetComponentType<GridBody>());

		m_InterpolStorage = ECSManager::Get()->GetComponentStorage<InterpolatedMovement>();
		m_RenderableStorage = ECSManager::Get()->GetComponentStorage<Renderable2D>();
		m_GridBodyStorage = ECSManager::Get()->GetComponentStorage<GridBody>();
	}
	void InterpolatedMovementSystem::Update(float dt)
	{
		for (auto& it : m_Components)
		{
			if (((*m_ActiveStorage)[it.activeIndex].activeComponents & m_Signature) == m_Signature)
			{
				if ((*m_InterpolStorage)[it.interpolIndex].inProgress)
				{
					float x = (*m_InterpolStorage)[it.interpolIndex].velocity.x * dt;
					float y = (*m_InterpolStorage)[it.interpolIndex].velocity.y * dt;

					if ((*m_InterpolStorage)[it.interpolIndex].distance.x > 0.0f)
					{
						(*m_InterpolStorage)[it.interpolIndex].distance.x -= fabs(x);
						(*m_RenderableStorage)[it.renderIndex].position.x += x;
					}
					if ((*m_InterpolStorage)[it.interpolIndex].distance.y > 0.0f)
					{
						(*m_InterpolStorage)[it.interpolIndex].distance.y -= fabs(y);
						(*m_RenderableStorage)[it.renderIndex].position.y += y;
					}

					if ((*m_InterpolStorage)[it.interpolIndex].distance.x <= 0.0f
						&& (*m_InterpolStorage)[it.interpolIndex].distance.y <= 0.0f)
					{
						(*m_InterpolStorage)[it.interpolIndex].inProgress = false;
						(*m_InterpolStorage)[it.interpolIndex].velocity = glm::vec2(0);

						(*m_InterpolStorage)[it.interpolIndex].distance.x = 0;
						(*m_InterpolStorage)[it.interpolIndex].distance.y = 0;						
					}
				}
				else if ((*m_GridBodyStorage)[it.gridBodyIndex].nextCol != 0
					  || (*m_GridBodyStorage)[it.gridBodyIndex].nextRow != 0)
				{
					(*m_InterpolStorage)[it.interpolIndex].distance.x = fabs((*m_GridBodyStorage)[it.gridBodyIndex].nextCol);
					(*m_InterpolStorage)[it.interpolIndex].distance.y = fabs((*m_GridBodyStorage)[it.gridBodyIndex].nextRow);
					(*m_InterpolStorage)[it.interpolIndex].inProgress = true;
				}
				
			}
		}
	}
	void InterpolatedMovementSystem::Add(Entity entity)
	{
		Component component;
		component.entity = entity;
		component.interpolIndex = ECSManager::Get()->GetComponentIndex<InterpolatedMovement>(entity);
		component.gridBodyIndex = ECSManager::Get()->GetComponentIndex<GridBody>(entity);
		component.renderIndex = ECSManager::Get()->GetComponentIndex<Renderable2D>(entity);
		component.activeIndex = ECSManager::Get()->GetComponentIndex<ActiveComponent>(entity);

		m_Components.push_back(component);
		XYZ_LOG_INFO("Entity with ID ", entity, " added");
	}
	void InterpolatedMovementSystem::Remove(Entity entity)
	{
		auto it = std::find(m_Components.begin(), m_Components.end(), entity);
		if (it != m_Components.end())
		{
			XYZ_LOG_INFO("Entity with id ", entity, " removed");
			*it = std::move(m_Components.back());
			m_Components.pop_back();
		}
	}
	bool InterpolatedMovementSystem::Contains(Entity entity)
	{
		auto it = std::find(m_Components.begin(), m_Components.end(), entity);
		if (it != m_Components.end())
			return true;
		return false;
	}
}