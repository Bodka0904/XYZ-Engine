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
	private:
		int binarySearch(int start, int end, Entity entity);
	private:
		struct SpriteComponent
		{
			SpriteAnimation* animation;
			Sprite* sprite;
			Entity entity;
		};

		struct Compare
		{
			bool operator()(const SpriteComponent& a, const SpriteComponent& b)
			{
				return a.entity < b.entity;
			}
		};

		std::vector<SpriteComponent> m_Components;

	};
}