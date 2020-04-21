#pragma once
#include "Types.h"

namespace XYZ {
	class SystemManager;
	class System
	{
		friend class SystemManager;
	public:
		virtual void Add(Entity entity) {};
		virtual void Remove(Entity entity) {};
		virtual void EntityUpdated(Entity entity) {};
		virtual bool Contains(Entity entity) = 0;

	protected:
		Signature m_Signature;

		struct Component
		{
			Entity entity;

			bool operator()(const Component& a, const Component& b)
			{
				return (a.entity < b.entity);
			}
			bool operator ==(const Entity other) const
			{
				return entity == other;
			}
			bool operator <(const Entity other) const
			{
				return entity < other;
			}
			bool operator >(const Entity other) const
			{
				return entity > other;
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