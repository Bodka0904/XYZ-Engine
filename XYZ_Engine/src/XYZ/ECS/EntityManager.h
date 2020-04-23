#pragma once
#include "Types.h"
#include "XYZ/Utils/DataStructures/FreeList.h"

#include <queue>
#include <array>

namespace XYZ {
	class EntityManager
	{
	public:
		EntityManager();

		Entity CreateEntity();

		Signature GetSignature(Entity entity);
		void DestroyEntity(Entity entity);
		void SetSignature(Entity entity, Signature signature);

	private:
		unsigned int m_EntitiesInUse;

		// Every entity has Signature representing components that it contains
		// System with same signatures will use entity
		FreeList<Signature> m_Signatures;
	};
}