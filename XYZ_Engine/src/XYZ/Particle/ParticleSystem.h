#pragma once
#include "XYZ/ECS/ECSManager.h"
#include "ParticleEffect.h"

#include "XYZ/Renderer/Shader.h"
#include "XYZ/Renderer/Material.h"


namespace XYZ {
	class ParticleSystem : public System
	{
	public:
		ParticleSystem();
		virtual void Update(float dt);
		virtual void Add(Entity entity) override;
		virtual void Remove(Entity entity) override;
	private:
		int binarySearch(int start, int end, Entity entity);
	private:
		struct ParticleComponent
		{
			Entity entity;
			ParticleEffect* effect;
		};
		struct Compare
		{
			bool operator()(const ParticleComponent& a, const ParticleComponent& b)
			{
				return (a.entity < b.entity);
			}
		};
		Compare m_Cmp;

		std::vector<ParticleComponent> m_Components;

		std::shared_ptr<Shader> m_TmpShader;
		std::shared_ptr<Material> m_TmpMaterial;
	};

}