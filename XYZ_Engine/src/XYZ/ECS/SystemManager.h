#pragma once
#include "System.h"
#include "Component.h"
#include "Types.h"

#include <memory>
#include <unordered_map>

namespace XYZ {
	
	class SystemManager
	{
	public:
		template<typename T>
		std::shared_ptr<T> RegisterSystem()
		{
			uint16_t id = System::GetID<T>();
			XYZ_ASSERT(m_Systems.find(id) == m_Systems.end(), "System already registered!");

			auto system = std::make_shared<T>();
			auto castedSystem = std::static_pointer_cast<System>(system);
			m_Systems.insert({ id,castedSystem });
			system->m_ActiveStorage = ECSManager::Get().GetComponentStorage<ActiveComponent>();
			return system;
		}

		template<typename T>
		void SetSignature(Signature signature)
		{
			uint16_t id = System::GetID<T>();
			XYZ_ASSERT(m_Systems.find(id) != m_Systems.end(), "System is not registered!");

			m_Systems[id]->m_Signature = signature;
		}
		template<typename T>
		std::shared_ptr<T> GetSystem()
		{
			uint16_t id = System::GetID<T>;
			XYZ_ASSERT(m_Systems.find(id) != m_Systems.end(), "System is not registered!");
			return std::static_pointer_cast<std::shared_ptr<T>>(m_Systems[id]);
		}
		void EntityDestroyed(Entity entity, Signature entitySignature)
		{
			// Erase a destroyed entity from all system lists
			for (auto const& it : m_Systems)
			{
				auto const& type = it.first;
				auto const& system = it.second;
				auto const& systemSignature = m_Systems[type]->m_Signature;
				
				if ((entitySignature & systemSignature) == systemSignature)
				{	
					it.second->Remove(entity);
				}
			}
		}

		void EntitySignatureChanged(Entity entity, Signature entitySignature)
		{
			for (auto const& pair : m_Systems)
			{
				auto const& type = pair.first;
				auto const& system = pair.second;
				auto const& systemSignature = m_Systems[type]->m_Signature;

				if ((entitySignature & systemSignature) == systemSignature)
				{
					if (!system->Contains(entity))
						system->Add(entity);
					
				}
				else 
					system->Remove(entity);

			}
		}

	private:
		std::unordered_map<uint16_t, std::shared_ptr<System>> m_Systems;
	};

}