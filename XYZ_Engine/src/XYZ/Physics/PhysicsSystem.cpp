#include "stdafx.h"
#include "PhysicsSystem.h"
#include "XYZ/Timer.h"

namespace XYZ {
	PhysicsSystem::PhysicsSystem()
	{
		m_Signature.set(XYZ::ECSManager::Get()->GetComponentType<XYZ::Transform2D>());
		m_Signature.set(XYZ::ECSManager::Get()->GetComponentType<XYZ::RigidBody2D>());
	}
	void PhysicsSystem::Update(float dt)
	{
		for (int i = 0; i < m_Components.size(); ++i)
		{
			m_Components[i].transform->pos += m_Components[i].rigidbody->velocity * dt;
			
			if (m_Components[i].transform->pos.y > 200.0f)
				m_Components[i].rigidbody->velocity.y -= m_Gravity;
			else
			{
				m_Components[i].rigidbody->velocity.y = 0.0f;
				m_Components[i].transform->pos.y = 200.0f;
			}		
		}
	}

	void PhysicsSystem::Add(Entity entity)
	{
		XYZ_LOG_INFO("Entity with ID ", entity, " added");

		PhysicsComponent pack;
		pack.entity = entity;
		pack.rigidbody = &ECSManager::Get()->GetComponent<RigidBody2D>(entity);
		pack.transform = &ECSManager::Get()->GetComponent<Transform2D>(entity);
		m_Components.push_back(pack);
	}

	void PhysicsSystem::Remove(Entity entity)
	{
		std::sort(m_Components.begin(), m_Components.end(), Compare());

		int position = binarySearch(0, (int)m_Components.size() - 1, entity);
		if (position != -1 && !m_Components.empty())
		{
			XYZ_LOG_INFO("Entity with ID ", entity, " removed");
			m_Components[position] = m_Components[m_Components.size() - 1];
			m_Components.erase(m_Components.end() - 1);
		}
	}

	int PhysicsSystem::binarySearch(int start, int end, Entity entity)
	{
		if (end >= start)
		{
			int mid = start + (end - start) / 2;
			if (m_Components[mid].entity == entity)
				return mid;

			if (m_Components[mid].entity > entity)
				return binarySearch(start, mid - 1, entity);


			return binarySearch(mid + 1, end, entity);
		}
		return -1;
	}
}