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
		}
	}

	void PhysicsSystem::Add(Entity entity)
	{
		XYZ_LOG_INFO("Entity with ID ", entity, " added");

		Component pack;
		pack.entity = entity;
		pack.rigidbody = &ECSManager::Get()->GetComponent<RigidBody2D>(entity);
		pack.transform = &ECSManager::Get()->GetComponent<Transform2D>(entity);
		m_Components.push_back(pack);
		m_EntitySorted = false;
	}

	void PhysicsSystem::Remove(Entity entity)
	{
		if (!m_EntitySorted)
		{
			std::sort(m_Components.begin(), m_Components.end(), Component());
			m_EntitySorted = true;
		}
		int position = binarySearch(0, (int)m_Components.size() - 1, entity,m_Components);
		if (position != -1 && !m_Components.empty())
		{
			XYZ_LOG_INFO("Entity with ID ", entity, " removed");
			m_Components[position] = m_Components[m_Components.size() - 1];
			m_Components.erase(m_Components.end() - 1);
			m_EntitySorted = false;
		}
	}

	bool PhysicsSystem::Contains(Entity entity)
	{
		if (!m_EntitySorted)
		{
			std::sort(m_Components.begin(), m_Components.end(), Component());
			m_EntitySorted = true;
		}
		int position = binarySearch(0, (int)m_Components.size() - 1, entity,m_Components);
		if (position == -1 || m_Components.empty())
			return false;
		return true;
	}

}