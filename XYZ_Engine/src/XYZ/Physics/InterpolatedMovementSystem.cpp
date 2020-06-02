#include "stdafx.h"
#include "InterpolatedMovementSystem.h"




namespace XYZ {
	InterpolatedMovementSystem::InterpolatedMovementSystem()
	{
		m_Signature.set(XYZ::ECSManager::Get().GetComponentType<InterpolatedMovement>());
		m_Signature.set(XYZ::ECSManager::Get().GetComponentType<Transform2D>());
		m_Signature.set(XYZ::ECSManager::Get().GetComponentType<GridBody>());
	}
	void InterpolatedMovementSystem::Update(float dt)
	{
		for (auto& it : m_Components)
		{
			if ((it.ActiveComponent.Get().ActiveComponents & m_Signature) == m_Signature)
			{
				if (it.Interpolated.Get().InProgress)
				{
					float x = it.Interpolated.Get().Velocity.x * dt;
					float y = it.Interpolated.Get().Velocity.y * dt;

					if (it.Interpolated.Get().Distance.x > 0.0f)
					{
						it.Interpolated.Get().Distance.x -= fabs(x);
						it.Transform.Get().GetPosition().x += x;
					}
					if (it.Interpolated.Get().Distance.y > 0.0f)
					{
						it.Interpolated.Get().Distance.y -= fabs(y);
						it.Transform.Get().GetPosition().y += y;
					}

					if (it.Interpolated.Get().Distance.x <= 0.0f
						&& it.Interpolated.Get().Distance.y <= 0.0f)
					{
						it.Interpolated.Get().InProgress = false;
						it.Interpolated.Get().Velocity = glm::vec2(0);

						it.Interpolated.Get().Distance.x = 0;
						it.Interpolated.Get().Distance.y = 0;						
					}
				}
				else if (it.GridBody.Get().NextCol != 0
					  || it.GridBody.Get().NextRow != 0)
				{
					it.Interpolated.Get().Distance.x = (float)fabs(it.GridBody.Get().NextCol);
					it.Interpolated.Get().Distance.y = (float)fabs(it.GridBody.Get().NextRow);
					it.Interpolated.Get().InProgress = true;
				}
				
			}
		}
	}
	void InterpolatedMovementSystem::Add(Entity entity)
	{
		Component component;
		component.ActiveComponent = ECSManager::Get().GetComponent<ActiveComponent>(entity);
		component.GridBody = ECSManager::Get().GetComponent<GridBody>(entity);
		component.Transform = ECSManager::Get().GetComponent<Transform2D>(entity);
		component.Interpolated = ECSManager::Get().GetComponent<InterpolatedMovement>(entity);

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