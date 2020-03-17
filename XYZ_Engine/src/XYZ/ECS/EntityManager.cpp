#include "stdafx.h"
#include "EntityManager.h"

namespace XYZ {
	EntityManager::EntityManager()
		: m_EntitiesInUse(0)
	{
		for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
		{
			m_AvailableEntities.push(entity);
		}
	}
	Entity EntityManager::CreateEntity()
	{
		XYZ_ASSERT(m_EntitiesInUse < MAX_ENTITIES, "Too many entities in existence.");

		// Take an ID from the front of the queue
		Entity id = m_AvailableEntities.front();
		m_AvailableEntities.pop();
		m_EntitiesInUse++;
		return id;
	}
	Signature EntityManager::GetSignature(Entity entity)
	{
		XYZ_ASSERT(entity < MAX_ENTITIES, "Entity out of range.");
		return m_Signatures[entity];
	}
	void EntityManager::DestroyEntity(Entity entity)
	{
		XYZ_ASSERT(entity < MAX_ENTITIES, "Entity out of range.");

		// Put the destroyed ID at the back of the queue
		//Restart bitset to zero;
		m_Signatures[entity].reset();
		m_AvailableEntities.push(entity);
		m_EntitiesInUse--;
	}
	void EntityManager::SetSignature(Entity entity, Signature signature)
	{
		XYZ_ASSERT(entity < MAX_ENTITIES, "Entity out of range.");

		// Put this entity's signature into the array
		m_Signatures[entity] = signature;
	}

}