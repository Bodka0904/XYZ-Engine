#pragma once
#include "ComponentManager.h"
#include "EntityManager.h"
#include "SystemManager.h"

#include <memory>

namespace XYZ {
	class ECSManager
	{
	public:
		ECSManager()
			: m_EntityEditing(false)
		{}
		void Init()
		{
			m_ComponentManager = std::make_unique<ComponentManager>();
			m_EntityManager = std::make_unique<EntityManager>();
			m_SystemManager = std::make_unique<SystemManager>();
		}
		template<typename T>
		std::shared_ptr<T> RegisterSystem()
		{
			return m_SystemManager->RegisterSystem<T>();
		}
		template<typename T>
		void RegisterComponent()
		{
			m_ComponentManager->RegisterComponent<T>();
		}
		template<typename T>
		void AddComponent(Entity entity, T* component)
		{
			m_ComponentManager->AddComponent<T>(entity, component);

			auto signature = m_EntityManager->GetSignature(entity);
			signature.set(m_ComponentManager->GetComponentType<T>(), 1);

			m_EntityManager->SetSignature(entity, signature);

		}
		template<typename T>
		void RemoveComponent(Entity entity)
		{
			m_ComponentManager->RemoveComponent<T>(entity);
			// Get signature of entity and update it after removing component
			auto signature = m_EntityManager->GetSignature(entity);
			// Set signature to zero 
			signature.set(m_ComponentManager->GetComponentType<T>(), 0);
			// Set updated signature for entity
			m_EntityManager->SetSignature(entity, signature);

			// Notify systems that entity was changed
			m_SystemManager->EntitySignatureChanged(entity, signature);
		}

		template<typename T>
		void SetSystemSignature(Signature signature)
		{
			m_SystemManager->SetSignature<T>(signature);
		}
		template<typename T>
		ComponentType GetComponentType()
		{
			return m_ComponentManager->GetComponentType<T>();
		}
		template<typename T>
		T& GetComponent(Entity entity)
		{
			return *m_ComponentManager->GetComponent<T>(entity);
		}

		template<typename T>
		std::shared_ptr<T> GetSystem()
		{
			return std::static_pointer_cast<T>(m_SystemManager->GetSystem<T>());
		}
		void SaveEntity(Entity entity)
		{
			m_EntityEditing = false;
			auto signature = m_EntityManager->GetSignature(entity);
			m_SystemManager->AddEntity(entity, signature);
		}
		void DestroyEntity(Entity entity)
		{
			auto signature = GetEntitySignature(entity);
			m_SystemManager->EntityDestroyed(entity, signature);
			m_ComponentManager->EntityDestroyed(entity);
			m_EntityManager->DestroyEntity(entity);
		}
		Signature GetEntitySignature(Entity entity)
		{
			return m_EntityManager->GetSignature(entity);
		}
		Entity CreateEntity()
		{
			XYZ_ASSERT(!m_EntityEditing, "Can not create new entity if there is unsaved one");
			m_EntityEditing = true;
			return m_EntityManager->CreateEntity();
		}

		static std::unique_ptr<ECSManager>& Get() { return m_Instance; }
	private:
		std::unique_ptr<ComponentManager> m_ComponentManager;
		std::unique_ptr<EntityManager>	  m_EntityManager;
		std::unique_ptr<SystemManager>	  m_SystemManager;
		bool							  m_EntityEditing;
		static std::unique_ptr<ECSManager> m_Instance;
	};

}