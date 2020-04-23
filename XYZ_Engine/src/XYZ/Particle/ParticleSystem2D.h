#pragma once
#include "XYZ/ECS/ECSManager.h"
#include "ParticleEffect2D.h"

#include "XYZ/Renderer/Shader.h"
#include "XYZ/Renderer/Material.h"


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
			int particleIndex;
			int numberOfChildren = 0;
			int childrenIndex[10];
		};

		std::vector<Component> m_Components;
		std::shared_ptr<ComponentStorage<ParticleEffect2D>> m_ParticleStorage;
		std::shared_ptr<ComponentStorage<ParticleEmitter>> m_EmitterStorage;
	};


}