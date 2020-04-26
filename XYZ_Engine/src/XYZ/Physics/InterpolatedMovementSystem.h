#pragma once
#include "XYZ/ECS/ECSManager.h"
#include "Components.h"
#include "XYZ/Renderer/Renderable2D.h"


namespace XYZ {
	/**
	* InterpolatedMovementystem takes care for interpolated movement of entities
	* with interpolated movement component
	*/
	class InterpolatedMovementSystem : public System
	{
	public:
		InterpolatedMovementSystem();
		virtual void Update(float dt);
		virtual void Add(Entity entity) override;
		virtual void Remove(Entity entity) override;
		virtual bool Contains(Entity entity) override;

	private:
		struct Component : public System::Component
		{
			int interpolIndex;
			int gridBodyIndex;
			int renderIndex;
		};

		std::shared_ptr<ComponentStorage<InterpolatedMovement>> m_InterpolStorage;
		std::shared_ptr<ComponentStorage<GridBody>> m_GridBodyStorage;
		std::shared_ptr<ComponentStorage<Renderable2D>> m_RenderableStorage;
		std::vector<Component> m_Components;
	};

}