#pragma once
#include "Types.h"
#include "XYZ/Utils/DataStructures/FreeList.h"

#include <unordered_map>
#include <array>
#include <assert.h>

namespace XYZ {
	class ComponentManager;
	class IComponent
	{
		friend class ComponentManager;
	public:
		virtual ~IComponent() = default;
		virtual void EntityDestroyed(Entity entity) = 0;

	private:
		ComponentType m_Type = 0;
	};

	template<typename T>
	class Component : public IComponent
	{
	public:
		virtual ~Component()
		{
			for (size_t i = 0; i < m_Components.Range(); i++)
				delete m_Components[i];
		}
		bool Contains(Entity entity)
		{
			return m_Lookup.find(entity) != m_Lookup.end();
		}

		void AddComponent(Entity entity, T* component)
		{
			XYZ_ASSERT(m_Lookup.find(entity) == m_Lookup.end(), "Entity already contains component");
			m_Lookup[entity] = m_Components.Insert(component);
		}
		void RemoveComponent(Entity entity)
		{		
			XYZ_ASSERT(m_Lookup.find(entity) != m_Lookup.end(), "Removing non-existent component");
			
			int removeIndex = m_Lookup[entity];
			delete m_Components[removeIndex];
			m_Components.Erase(removeIndex);
			m_Lookup.erase(entity);			
		}
		T* GetComponent(Entity entity)
		{
			XYZ_ASSERT(m_Lookup.find(entity) != m_Lookup.end(), "Retrieving non-existent component.");
			return m_Components[m_Lookup[entity]];
		}

		virtual void EntityDestroyed(Entity entity) override
		{
			RemoveComponent(entity);
		}

	private:
		FreeList<T*> m_Components;
		std::unordered_map<Entity, int> m_Lookup;
	};

}