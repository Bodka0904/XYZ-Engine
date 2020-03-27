#pragma once
#include "XYZ/ECS/ECSManager.h"
#include "SpriteAnimation.h"
#include "Sprite.h"

namespace XYZ {
	class SpriteSystem : public System
	{
	public:
		SpriteSystem();
		virtual void Update(float dt);
		virtual void Add(Entity entity) override;
		virtual void Remove(Entity entity) override;
		virtual bool Contains(Entity entity) override;

	private:
		struct Component : public System::Component
		{
			SpriteAnimation* animation;
			Sprite* sprite;
		};

		std::vector<Component> m_Components;

	};
}