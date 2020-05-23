#pragma once
#include "Types.h"

namespace XYZ {
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
	public:
		uint16_t GetComponentID() const override
		{
			return IComponent::GetID<Derived>();
		}
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