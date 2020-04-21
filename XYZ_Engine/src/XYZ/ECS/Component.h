#pragma once

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
}