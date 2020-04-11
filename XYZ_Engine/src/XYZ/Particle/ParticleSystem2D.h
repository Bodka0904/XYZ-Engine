#pragma once
#include "XYZ/ECS/ECSManager.h"
#include "ParticleEffect2D.h"

#include "XYZ/Renderer/Shader.h"
#include "XYZ/Renderer/Material.h"

#include <set>
#include <unordered_set>

namespace XYZ {
	class ParticleSystem2D : public System
	{
	public:
		ParticleSystem2D();
		void Update(float dt);
		virtual void Add(Entity entity) override;
		virtual void Remove(Entity entity) override;
		virtual bool Contains(Entity entity)override;

	private:
		struct Component : public System::Component
		{
			ParticleEffect2D* effect;
		};

		std::vector<Component> m_Components;
	};

}