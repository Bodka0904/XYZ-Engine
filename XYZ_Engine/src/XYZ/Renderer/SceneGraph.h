#pragma once
#include "XYZ/ECS/ECSManager.h"
#include "XYZ/Physics/Components.h"

namespace XYZ {

	class SceneGraph
	{
	public:
		SceneGraph();

		void Insert(Entity entity);
		void Remove(Entity entity);

		void AttachChild(Entity parent, Entity children);
		void DetachChild(Entity parent, Entity children);

		// Debug
		void Print();
	private:
		struct Component
		{
			Entity entity;

			int activeIndex;
			// data
			int transformIndex;
			
			// linkage
			int relationIndex = -1;
		};


		std::vector<Component> m_Components;

		std::shared_ptr<ComponentStorage<Transform2D>> m_TransformStorage;
		std::shared_ptr<ComponentStorage<Relationship>> m_RelationStorage;


	
		struct ParentComparator
		{
			ParentComparator()
			{

			}

			bool operator()(const Component& a, const Component& b) const
			{

			}
		};
	};
}