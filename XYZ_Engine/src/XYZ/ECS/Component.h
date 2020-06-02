#pragma once
#include "Types.h"
#include "ComponentStorage.h"

namespace XYZ {

	// Controls if component was invalidated in memory
	// Always use for storing components
	template <typename T>
	class ComponentWrapper
	{
	public:
		ComponentWrapper(T* component, Entity entity)
			:
			m_Component(component),
			m_Entity(entity)
		{}
		ComponentWrapper() = default;


		const T& Get() const
		{
			XYZ_ASSERT(m_Component, "Wrapper contains null pointer to component");
			XYZ_ASSERT(m_Component->m_IsValid, "Component is not valid");		
			return *m_Component;
		}

		T& Get()
		{
			XYZ_ASSERT(m_Component, "Wrapper contains null pointer to component");
			// If not valid get new pointer from storage
			if (!m_Component->m_IsValid)
			{
				auto storage = ECSManager::Get().GetComponentStorage<T>();
				m_Component = &storage->GetComponent(m_Entity);
				m_Component->m_IsValid = true;
			}
			return *m_Component;
		}

		Entity GetEntity() const
		{
			return m_Entity;
		}

	private:
		T* m_Component;
		Entity m_Entity;
	};


	class ComponentManager;
	/* !@class IComponent
	* @brief interface of component
	*/
	class IComponent
	{
		friend class ComponentManager;
	public:
		virtual uint16_t GetComponentID() const = 0;

	protected:
		// Unique static ID
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

	/* !@class Type
	* @brief type of component
	*/
	template <typename Derived, typename DeriveFrom = IComponent>
	class Type : public IComponent
	{
		// Wrapper need access to m_Valid variable when it validates pointer to component
		friend class ComponentWrapper<Derived>;
		// Storage need access to m_Valid variable when it invalidates pointer to component
		friend class ComponentStorage<Derived>;
	public:
		// return unique static id
		virtual uint16_t GetComponentID() const override
		{
			return IComponent::GetID<Derived>();
		}

	private:
		// True if invalidated in container
		bool m_IsValid = true;
	};


	struct ActiveComponent : public Type<ActiveComponent>
	{
		Signature ActiveComponents;
	};

	struct Relationship : public Type<Relationship>
	{
		size_t NumChildren = 0;
		Entity Parent = NULL_ENTITY;
		Entity Previous = NULL_ENTITY;
		Entity Next = NULL_ENTITY;
		Entity First = NULL_ENTITY;
	};
}