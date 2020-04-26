#pragma once
#include "XYZ/ECS/ECSManager.h"
#include "Renderable2D.h"
#include "SortingGroup.h"


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
		struct ZAscend
		{
			ZAscend()
			{
				m_Storage = ECSManager::Get()->GetComponentStorage<Renderable2D>();
			}
			std::shared_ptr<ComponentStorage<Renderable2D>>m_Storage;
			bool operator()(int a, int b) const
			{
				return (*m_Storage)[a].position.z <= (*m_Storage)[b].position.z;
			}
		};
		struct ZDescend
		{
			ZDescend()
			{
				m_Storage = ECSManager::Get()->GetComponentStorage<Renderable2D>();
			}
			std::shared_ptr<ComponentStorage<Renderable2D>>m_Storage;
			bool operator()(int a, int b) const
			{
				return (*m_Storage)[a].position.z >= (*m_Storage)[b].position.z;
			}
		};
	private:
		struct Component : public System::Component
		{
			int64_t key;
		};
		std::vector<Component> m_Components;
		std::shared_ptr<ComponentStorage<Renderable2D>> m_Storage;

		/** Transparent group z ascend ordering*/ 
		SortingGroup<int,ZAscend> m_TransparentGroup;

		/** Opaque group z descend ordering*/
		SortingGroup<int, ZDescend> m_OpaqueGroup;
	};
}