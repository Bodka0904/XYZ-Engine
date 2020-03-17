#include "stdafx.h"
#include "ParticleSystem.h"

namespace XYZ {
	ParticleSystem::ParticleSystem()
	{
		m_TmpMaterial = Material::Create(Shader::Create("../XYZ_Engine/Assets/Shaders/Particle/SubRoutineTestComputeShader.glsl"));
		m_TmpMaterial->Set("u_Speed", 5.0f);
		m_TmpMaterial->Set("u_Gravity", -6.8f);
		m_TmpMaterial->SetRoutine("blueColor");

		m_Signature.set(XYZ::ECSManager::Get()->GetComponentType<XYZ::ParticleEffect>());
	}

	void ParticleSystem::Update(float dt)
	{
		m_TmpMaterial->Set("u_Time", dt);
		m_TmpMaterial->Bind();
		for (int i = 0; i < m_Components.size(); ++i)
		{
			m_TmpMaterial->Set("u_Emitter", m_Components[i].effect->emitter);
			m_TmpMaterial->Set("u_NumberRows", (float)m_Components[i].effect->textureRows);
			m_TmpMaterial->Set("u_NumberColumns", (float)m_Components[i].effect->textureColumns);
			m_Components[i].effect->Bind();
			m_TmpMaterial->GetShader()->Compute(32, 32, 1);
		}
	}

	void ParticleSystem::Add(Entity entity)
	{
		XYZ_LOG_INFO("Entity with id ", entity, " added");
		ParticleComponent component;
		component.effect = &ECSManager::Get()->GetComponent<ParticleEffect>(entity);
		component.entity = entity;
		m_Components.push_back(component);
	}

	void ParticleSystem::Remove(Entity entity)
	{
		std::sort(m_Components.begin(), m_Components.end(), m_Cmp);

		int position = binarySearch(0, (int)m_Components.size() - 1, entity);
		if (position != -1 && !m_Components.empty())
		{
			XYZ_LOG_INFO("Entity with id ", entity, " removed");
			m_Components[position] = m_Components[m_Components.size() - 1];
			m_Components.erase(m_Components.end() - 1);
		}
	}

	int ParticleSystem::binarySearch(int start, int end, Entity entity)
	{
		if (end >= start)
		{
			int mid = start + (end - start) / 2;
			if (m_Components[mid].entity == entity)
				return mid;

			if (m_Components[mid].entity > entity)
				return binarySearch(start, mid - 1, entity);


			return binarySearch(mid + 1, end, entity);
		}
		return -1;
	}
}