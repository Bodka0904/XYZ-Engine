#pragma once
#include "XYZ/ECS/ECSManager.h"
#include "Renderable2D.h"
#include "SortingGroup.h"


namespace XYZ {

	class RendererBatchSystem2D : public System
	{
	public:
		RendererBatchSystem2D();

		virtual void Add(Entity entity);
		virtual void Remove(Entity entity);
		virtual bool Contains(Entity entity);

		void SubmitToRenderer();
	private:
		struct ZAscend
		{
			bool operator()(const Renderable2D* a, const Renderable2D* b)
			{
				return a->position.z + a->material->GetSortKey() < b->position.z + b->material->GetSortKey();
			}
		};
		struct ZDescend
		{
			bool operator()(const Renderable2D* a, const Renderable2D* b)
			{
				return a->position.z + a->material->GetSortKey() > b->position.z + b->material->GetSortKey();
			}
		};
	private:

		std::vector<Entity> m_Entities;
	
		SortingGroup<ZAscend> m_TransparentGroup;
		SortingGroup<ZDescend> m_OpaqueGroup;
	};
}