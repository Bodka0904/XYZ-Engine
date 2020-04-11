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

	}

	void PhysicsSystem::Remove(Entity entity)
	{
		auto it = std::find(m_Components.begin(), m_Components.end(), entity);
		if (it != m_Components.end())
		{
			XYZ_LOG_INFO("Entity with id ", entity, " removed");
			it = m_Components.end() - 1;
			m_Components.erase(m_Components.end() - 1);
		}
	}

	bool PhysicsSystem::Contains(Entity entity)
	{
		auto it = std::find(m_Components.begin(), m_Components.end(), entity);
		if (it != m_Components.end())
			return true;
		return false;
	}

}