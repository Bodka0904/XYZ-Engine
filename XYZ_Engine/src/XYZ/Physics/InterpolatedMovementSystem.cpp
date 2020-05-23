#include "stdafx.h"
#include "InterpolatedMovementSystem.h"




namespace XYZ {
	InterpolatedMovementSystem::InterpolatedMovementSystem()
	{
		m_Signature.set(XYZ::ECSManager::Get().GetComponentType<InterpolatedMovement>());
		m_Signature.set(XYZ::ECSManager::Get().GetComponentType<Transform2D>());
		m_Signature.set(XYZ::ECSManager::Get().GetComponentType<GridBody>());

		m_InterpolStorage = ECSManager::Get().GetComponentStorage<InterpolatedMovement>();
		m_TransformStorage = ECSManager::Get().GetComponentStorage<Transform2D>();
		m_GridBodyStorage = ECSManager::Get().GetComponentStorage<GridBody>();
	}
	void InterpolatedMovementSystem::Update(float dt)
	{
		for (auto& it : m_Components)
		{
			if (((*m_ActiveStorage)[it.ActiveIndex].ActiveComponents & m_Signature) == m_Signature)
			{
				if ((*m_InterpolStorage)[it.InterpolIndex].InProgress)
				{
					float x = (*m_InterpolStorage)[it.InterpolIndex].Velocity.x * dt;
					float y = (*m_InterpolStorage)[it.InterpolIndex].Velocity.y * dt;

					if ((*m_InterpolStorage)[it.InterpolIndex].Distance.x > 0.0f)
					{
						(*m_InterpolStorage)[it.InterpolIndex].Distance.x -= fabs(x);
						(*m_TransformStorage)[it.TransformIndex].Position.x += x;
					}
					if ((*m_InterpolStorage)[it.InterpolIndex].Distance.y > 0.0f)
					{
						(*m_InterpolStorage)[it.InterpolIndex].Distance.y -= fabs(y);
						(*m_TransformStorage)[it.TransformIndex].Position.y += y;
					}

					if ((*m_InterpolStorage)[it.InterpolIndex].Distance.x <= 0.0f
						&& (*m_InterpolStorage)[it.InterpolIndex].Distance.y <= 0.0f)
					{
						(*m_InterpolStorage)[it.InterpolIndex].InProgress = false;
						(*m_InterpolStorage)[it.InterpolIndex].Velocity = glm::vec2(0);

						(*m_InterpolStorage)[it.InterpolIndex].Distance.x = 0;
						(*m_InterpolStorage)[it.InterpolIndex].Distance.y = 0;						
					}
				}
				else if ((*m_GridBodyStorage)[it.GridBodyIndex].NextCol != 0
					  || (*m_GridBodyStorage)[it.GridBodyIndex].NextRow != 0)
				{
					(*m_InterpolStorage)[it.InterpolIndex].Distance.x = (float)fabs((*m_GridBodyStorage)[it.GridBodyIndex].NextCol);
					(*m_InterpolStorage)[it.InterpolIndex].Distance.y = (float)fabs((*m_GridBodyStorage)[it.GridBodyIndex].NextRow);
					(*m_InterpolStorage)[it.InterpolIndex].InProgress = true;
				}
				
			}
		}
	}
	void InterpolatedMovementSystem::Add(Entity entity)
	{
		Component component;
		component.Ent = entity;
		component.InterpolIndex = ECSManager::Get().GetComponentIndex<InterpolatedMovement>(entity);
		component.GridBodyIndex = ECSManager::Get().GetComponentIndex<GridBody>(entity);
		component.TransformIndex = ECSManager::Get().GetComponentIndex<Transform2D>(entity);
		component.ActiveIndex = ECSManager::Get().GetComponentIndex<ActiveComponent>(entity);

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