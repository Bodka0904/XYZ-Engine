#pragma once
#include "XYZ/ECS/ECSManager.h"
#include "XYZ/Physics/Components.h"
#include "Renderable2D.h"
#include "SortingLayer.h"


#include <queue>

namespace XYZ {
	

	/**
	* @class RendererBatchSystem2D
	* @brief represents system , that groups ( batches ) renderables dependent on the material and z distance from the camera
	*/
	class RendererBatchSystem2D : public System
	{
	public:
		/**
		* Construct a batch system
		*/
		RendererBatchSystem2D();

		/**
		* Add entity to the Opaque/Transparent group dependent on the sort key in the material of the entity
		* @param[in] enity 
		*/
		virtual void Add(Entity entity) override;

		/**
		* Remove entity
		* @param[in] entity
		*/
		virtual void Remove(Entity entity) override;

		/**
		* Check if system contains entity
		* @param[in] entity
		*/
		virtual bool Contains(Entity entity) override;

		/**
		* If entity is updated, if the sort key of the entity has changed, remove it from the current group,
		* and insert it to the new group dependent on the key
		* @param[in] entity
		*/
		virtual void EntityUpdated(Entity entity) override;

		/**
		* Build commands and submit them to the Renderer2D
		*/
		void SubmitToRenderer();
	


	private:
	

		struct Component : public System::Component
		{
			int RenderableIndex;
			int TransformIndex;
		};

		std::vector<Component> m_OpaqueComponents;
		std::vector<Component> m_TransparentComponents;


		std::shared_ptr<ComponentStorage<Renderable2D>> m_RenderableStorage;
		std::shared_ptr<ComponentStorage<Transform2D>> m_TransformStorage;

		struct OpaqueComparator
		{
			OpaqueComparator()
				:
				m_TransformStorage(ECSManager::Get().GetComponentStorage<Transform2D>()),
				m_RenderableStorage(ECSManager::Get().GetComponentStorage<Renderable2D>())
			{
			}
			std::shared_ptr<ComponentStorage<Transform2D>> m_TransformStorage;
			std::shared_ptr<ComponentStorage<Renderable2D>> m_RenderableStorage;

			bool operator()(const Component& a, const Component& b) const
			{
				auto& transformA = (*m_TransformStorage)[a.TransformIndex];
				auto& transformB = (*m_TransformStorage)[b.TransformIndex];
				auto& renderableA = (*m_RenderableStorage)[a.RenderableIndex];
				auto& renderableB = (*m_RenderableStorage)[b.RenderableIndex];

				int sortLayerA = renderableA.SortLayerID;
				int sortLayerB = renderableB.SortLayerID;

				float valueA = (float)SortingLayer::Get().GetOrderValueByID(sortLayerA) + transformA.Position.z;
				float valueB = (float)SortingLayer::Get().GetOrderValueByID(sortLayerB) + transformB.Position.z;


				if (renderableA.Material->GetSortKey() == renderableB.Material->GetSortKey())
				{
					return valueA < valueB;
				}
				return renderableA.Material->GetSortKey() < renderableB.Material->GetSortKey();		
			}
		};

		struct TransparentComparator
		{
			TransparentComparator()
				:
				m_TransformStorage(ECSManager::Get().GetComponentStorage<Transform2D>()),
				m_RenderableStorage(ECSManager::Get().GetComponentStorage<Renderable2D>())
			{
			}
			std::shared_ptr<ComponentStorage<Transform2D>> m_TransformStorage;
			std::shared_ptr<ComponentStorage<Renderable2D>> m_RenderableStorage;

			bool operator()(const Component& a, const Component& b) const
			{
				auto& transformA = (*m_TransformStorage)[a.TransformIndex];
				auto& transformB = (*m_TransformStorage)[b.TransformIndex];
				auto& renderableA = (*m_RenderableStorage)[a.RenderableIndex];
				auto& renderableB = (*m_RenderableStorage)[b.RenderableIndex];

				int sortLayerA = renderableA.SortLayerID;
				int sortLayerB = renderableB.SortLayerID;

				float valueA = (float)SortingLayer::Get().GetOrderValueByID(sortLayerA) + transformA.Position.z;
				float valueB = (float)SortingLayer::Get().GetOrderValueByID(sortLayerB) + transformB.Position.z;

				if (fabs(valueA - valueB) <= std::numeric_limits<float>::epsilon())
				{
					return renderableA.Material->GetSortKey() < renderableB.Material->GetSortKey();
				}
				return valueA > valueB;
			}
		};

	private:	

		// Maybe useless
		template <typename Comparator>
		void InsertionSort(std::vector<Component>& vec)
		{
			Comparator cmp;
			for (int i = 1; i < vec.size(); ++i)
			{
				auto el = vec[i];
				int j = i - 1;

				while (j >= 0 && cmp(el, vec[j]))
				{
					vec[j + 1] = vec[j];
					j = j - 1;
				}
				vec[j + 1] = el;
			}
		}
	};
}