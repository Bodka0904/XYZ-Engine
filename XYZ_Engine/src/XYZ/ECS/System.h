#pragma once
#include "Component.h"
#include "ComponentStorage.h"

namespace XYZ {
	class SystemManager;

	/*! @class System
	* @brief Interface for systems
	*/
	class System
	{
		friend class SystemManager;
	public:
		/**
		* Add entity to the system
		* @param[in] entity
		*/
		virtual void Add(Entity entity) {};

		/**
		* Remove entity from the system
		* @param[in] entity
		*/
		virtual void Remove(Entity entity) {};
		
		/**
		* Remove/Add entity from the system
		* @param[in] entity
		*/
		virtual void EntityUpdated(Entity entity) {};
		
		/**
		* Check if system contains entity
		* @param[in] entity
		* @return 
		*/
		virtual bool Contains(Entity entity) = 0;

	protected:
		Signature m_Signature;

		struct Component
		{
			ComponentWrapper<ActiveComponent> ActiveComponent;

			bool operator()(const Component& a, const Component& b)
			{
				return (ActiveComponent.GetEntity() < ActiveComponent.GetEntity());
			}
			bool operator ==(const Entity other) const
			{
				return ActiveComponent.GetEntity() == other;
			}
			bool operator <(const Entity other) const
			{
				return ActiveComponent.GetEntity() < other;
			}
			bool operator >(const Entity other) const
			{
				return ActiveComponent.GetEntity() > other;
			}
		};
	protected:
		template <typename T>
		static uint16_t GetID()
		{
			static uint16_t compType = getNextID();
			return compType;
		}
	private:
		static uint16_t getNextID()
		{
			static uint16_t nextType = 0;
			return ++nextType;
		}
	};

}