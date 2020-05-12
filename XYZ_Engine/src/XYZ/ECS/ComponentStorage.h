#pragma once
#include "Types.h"
#include "XYZ/Utils/DataStructures/FreeList.h"

#include <unordered_map>
#include <array>
#include <assert.h>

namespace XYZ {
	class ComponentManager;
	class IComponentStorage
	{
		friend class ComponentManager;
	public:
		virtual ~IComponentStorage() = default;
		virtual void EntityDestroyed(Entity entity) = 0;

	};

	template<typename T>
	class ComponentStorage : public IComponentStorage
	{
	public:
		virtual ~ComponentStorage()
		{
		
		}
		bool Contains(Entity entity)
		{
			return m_Lookup.find(entity) != m_Lookup.end();
		}

		void AddComponent(Entity entity,const T& component)
		{
			XYZ_ASSERT(m_Lookup.find(entity) == m_Lookup.end(), "Entity ",entity," already contains component");
			m_Lookup[entity] = m_Components.Insert(component);
		}

		void RemoveComponent(Entity entity)
		{
			XYZ_ASSERT(m_Lookup.find(entity) != m_Lookup.end(), "Removing non-existent component");
			int removeIndex = m_Lookup[entity];	
			m_Components.Erase(removeIndex);
			m_Lookup.erase(entity);
		}

		int GetComponentIndex(Entity entity)
		{
			XYZ_ASSERT(m_Lookup.find(entity) != m_Lookup.end(), "Retrieving non-existent component.");
			return m_Lookup[entity];
		}

		T* GetComponent(Entity entity)
		{
			XYZ_ASSERT(m_Lookup.find(entity) != m_Lookup.end(), "Retrieving non-existent component.");
			return &m_Components[m_Lookup[entity]];
		}

		virtual void EntityDestroyed(Entity entity) override
		{
			if (Contains(entity))
				RemoveComponent(entity);
		}

		T& operator [](int index)
		{
			XYZ_ASSERT(index < m_Components.Range() && index >= 0, "Index out of range");
			return m_Components[index];
		}


	private:
		FreeList<T> m_Components;
		std::unordered_map<Entity, int> m_Lookup;
	};

}