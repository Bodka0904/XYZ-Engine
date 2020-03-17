#pragma once
#include "Types.h"

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
			for (size_t i = 0; i < m_Component.size(); i++)
				delete m_Component[i];
		}
		bool Contains(Entity entity)
		{
			return m_Lookup.find(entity) != m_Lookup.end();
		}

		void AddComponent(Entity entity, T* component)
		{
			XYZ_ASSERT(m_Lookup.find(entity) == m_Lookup.end(), "Entity already contains component");
			m_Lookup[entity] = m_Num;
			m_Component[m_Num] = component;

			m_Num++;
		}
		void RemoveComponent(Entity entity)
		{
			if (m_Num > 0)
			{
				XYZ_ASSERT(m_Lookup.find(entity) != m_Lookup.end(), "Removing non-existent component");
				size_t indexRemovedEnity = m_Lookup[entity];
				delete m_Component[indexRemovedEnity];

				m_Component[indexRemovedEnity] = m_Component[m_Num - 1];
				m_Lookup[m_Num - 1] = indexRemovedEnity;

				m_Lookup.erase(entity);
				m_Num--;
			}
		}
		T* GetComponent(Entity entity)
		{
			XYZ_ASSERT(m_Lookup.find(entity) != m_Lookup.end(), "Retrieving non-existent component.");
			return m_Component[m_Lookup[entity]];
		}

		virtual void EntityDestroyed(Entity entity) override
		{
			RemoveComponent(entity);
		}

	private:
		std::array<T*, MAX_ENTITIES> m_Component;
		std::unordered_map<Entity, size_t> m_Lookup;

		unsigned int m_Num = 0;
	};

}