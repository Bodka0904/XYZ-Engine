#pragma once
#include "XYZ/ECS/ECSManager.h"
#include "Components.h"


namespace XYZ {
	/*! @class InterpolatedMovementSystem
	* @brief InterpolatedMovementystem takes care for interpolated movement of entities with interpolated movement component
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
			ComponentWrapper<InterpolatedMovement> Interpolated;
			ComponentWrapper<GridBody> GridBody;
			ComponentWrapper<Transform2D> Transform;
		};

		std::vector<Component> m_Components;
	};

}