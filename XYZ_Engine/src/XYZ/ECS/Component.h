#pragma once
#include "Types.h"

namespace XYZ {
	class ComponentManager;
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
		Signature activeComponents;
	};

	struct ParentComponent : public Type<ParentComponent>
	{
		ParentComponent(Entity Parent)
			: parentEntity(Parent)
		{}
		Entity parentEntity;
	};

	struct ChildrenComponent : public Type<ChildrenComponent>
	{
		static constexpr int sc_MaxChildren = 10;
		std::vector<Entity> children;
	};
}