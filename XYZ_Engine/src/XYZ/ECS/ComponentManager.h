#pragma once
#include "Types.h"
#include "Component.h"
#include "XYZ/Core/Core.h"

#include <unordered_map>
#include <memory>

namespace XYZ {
	class ComponentManager
	{
	public:
		template<typename T>
		void RegisterComponent()
		{
			const char* typeName = typeid(T).name();
			XYZ_ASSERT(m_Components.find(typeName) == m_Components.end(), "Registering component type more than once.");

			std::shared_ptr<Component<T>> comp = std::make_shared< Component<T> >();
			m_Components.insert({ typeName,comp });
			m_Components[typeName]->m_Type = m_NextComponentType;

			m_NextComponentType++;
		}
		template<typename T>
		void AddComponent(Entity entity, T* component)
		{
			const char* typeName = typeid(T).name();
			XYZ_ASSERT(m_Components.find(typeName) != m_Components.end(), "Accessing not registered component.");
			GetComponentArray<T>()->AddComponent(entity, component);
		}

		template<typename T>
		ComponentType GetComponentType()
		{
			const char* typeName = typeid(T).name();

			XYZ_ASSERT(m_Components.find(typeName) != m_Components.end(), "Component not registered before use.");

			return m_Components[typeName]->m_Type;
		}

		template<typename T>
		T* GetComponent(Entity entity)
		{
			return GetComponentArray<T>()->GetComponent(entity);
		}

		template<typename T>
		void RemoveComponent(Entity entity)
		{
			GetComponentArray<T>()->RemoveComponent(entity);
		}

		void EntityDestroyed(Entity entity)
		{
			for (auto const& it : m_Components)
			{
				it.second->EntityDestroyed(entity);
			}
		}

	private:
		std::unordered_map<std::string, std::shared_ptr<IComponent> > m_Components;
		ComponentType m_NextComponentType = 0;

	private:
		template<typename T>
		std::shared_ptr<Component<T>> GetComponentArray()
		{
			const char* typeName = typeid(T).name();

			XYZ_ASSERT(m_Components.find(typeName) != m_Components.end(), "Component not registered before use.");

			return std::static_pointer_cast<Component<T>>(m_Components[typeName]);
		}
	};

}