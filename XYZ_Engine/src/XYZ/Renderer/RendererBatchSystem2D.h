#pragma once
#include "XYZ/ECS/ECSManager.h"
#include "Renderable2D.h"
#include "SortingGroup.h"


namespace XYZ {

	class RendererBatchSystem2D : public System
	{
	public:
		RendererBatchSystem2D();

		virtual void Add(Entity entity) override;
		virtual void Remove(Entity entity) override;
		virtual bool Contains(Entity entity) override;
		virtual void EntityUpdated(Entity entity) override;

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
		struct Component : public System::Component
		{
			int64_t key;
		};
		std::vector<Component> m_Components;

		SortingGroup<ZAscend> m_TransparentGroup;
		SortingGroup<ZDescend> m_OpaqueGroup;
	};
}