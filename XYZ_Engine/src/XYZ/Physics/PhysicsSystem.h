#pragma once
#include "XYZ/ECS/ECSManager.h"
#include "Components.h"
#include "Grid2D.h"

#include "XYZ/Renderer/Renderable2D.h"


namespace XYZ {
	/**
	* @class PhysicsSytem
	* @brief Represents system for handling physics
	*/
	class PhysicsSystem : public System
	{
	public:
		/**
		* Construct
		*/
		PhysicsSystem();
		~PhysicsSystem();

		virtual void Update(float dt);
		virtual void Add(Entity entity) override;
		virtual void Remove(Entity entity) override;
		virtual bool Contains(Entity entity) override;

		void CreateGrid(int numRows, int numCols, int cellSize);
	
	private:
		struct Component : public System::Component
		{
			int renderIndex;
			int bodyIndex;
			int gridPosIndex;
			int interpolIndex;
			int layerIndex;
		};
	private:
		void AnimateMovement(const Component& it, float dt);

	private:
		std::vector<Component> m_Components;
		std::shared_ptr<ComponentStorage<Renderable2D>> m_RenderableStorage;
		std::shared_ptr<ComponentStorage<RigidBody2D>> m_BodyStorage;
		std::shared_ptr<ComponentStorage<GridPosition>> m_GridPosStorage;
		std::shared_ptr<ComponentStorage<InterpolatedMovement>> m_InterpolStorage;
		std::shared_ptr<ComponentStorage<LayerMask>> m_LayerStorage;

		Grid2D *m_Grid;
	};

}