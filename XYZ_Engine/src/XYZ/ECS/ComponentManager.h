#pragma once
#include "Types.h"
#include "Component.h"
#include "ComponentStorage.h"
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
			uint16_t id = IComponent::GetID<T>();
			XYZ_ASSERT(id < MAX_COMPONENTS, "Registering more than max components");
			XYZ_ASSERT(m_Components.find(id) == m_Components.end(), "Registering component type more than once.");

			std::shared_ptr<IComponentStorage> componentStorage = std::make_shared<ComponentStorage<T> >();
			m_Components.insert({ id,componentStorage });
		}
		template<typename T>
		void AddComponent(Entity entity,const T& component)
		{
			uint16_t id = IComponent::GetID<T>();
			XYZ_ASSERT(m_Components.find(id) != m_Components.end(), "Accessing not registered component.");
			GetComponentStorage<T>()->AddComponent(entity, component);
		}

		void AddRelation(Entity entity, const ParentComponent& component)
		{
			uint16_t id = IComponent::GetID<ParentComponent>();
			XYZ_ASSERT(m_Components.find(id) != m_Components.end(), "Accessing not registered component.");
			id = IComponent::GetID<ChildrenComponent>();
			XYZ_ASSERT(m_Components.find(id) != m_Components.end(), "Accessing not registered component.");
			GetComponentStorage<ChildrenComponent>()->AddChild(component.parentEntity, entity);
			GetComponentStorage<ParentComponent>()->AddComponent(entity, component);
		}

		template<typename T>
		ComponentType GetComponentType()
		{
			uint16_t id = IComponent::GetID<T>();
			XYZ_ASSERT(m_Components.find(id) != m_Components.end(), "Component not registered before use.");
			return id;
		}

		template<typename T>
		std::shared_ptr<ComponentStorage<T>> GetComponentStorage()
		{
			uint16_t id = IComponent::GetID<T>();
			XYZ_ASSERT(m_Components.find(id) != m_Components.end(), "Component not registered before use.");
			return std::static_pointer_cast<ComponentStorage<T>>(m_Components[id]);
		}

		template<typename T>
		T* GetComponent(Entity entity)
		{
			return GetComponentStorage<T>()->GetComponent(entity);
		}

		template <typename T>
		int GetComponentIndex(Entity entity)
		{
			return GetComponentStorage<T>()->GetComponentIndex(entity);
		}
		template<typename T>
		void RemoveComponent(Entity entity)
		{
			GetComponentStorage<T>()->RemoveComponent(entity);
		}

		template <typename T>
		bool Contains(Entity entity)
		{
			return GetComponentStorage<T>()->Contains(entity);
		}

		void EntityDestroyed(Entity entity)
		{
			for (auto const& it : m_Components)
			{
				it.second->EntityDestroyed(entity);
			}
		}

	private:
		std::unordered_map<uint16_t, std::shared_ptr<IComponentStorage> > m_Components;

		
	};

}