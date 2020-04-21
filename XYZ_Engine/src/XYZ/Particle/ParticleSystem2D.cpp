#include "stdafx.h"
#include "ParticleSystem2D.h"
#include "XYZ/Renderer/Renderable2D.h"

namespace XYZ {
	ParticleSystem2D::ParticleSystem2D()
	{
		m_Signature.set(XYZ::ECSManager::Get()->GetComponentType<XYZ::ParticleEffect2D>());
		m_ParticleStorage = ECSManager::Get()->GetComponentStorage<ParticleEffect2D>();
	}

	void ParticleSystem2D::Update(float dt)
	{
		int16_t currentKey = 0;
		for (auto it : m_Components)
		{
			int16_t newKey = (*m_ParticleStorage)[it.index].GetMaterial()->GetSortKey();
			if (currentKey != newKey)
			{
				currentKey = newKey;
				(*m_ParticleStorage)[it.index].GetMaterial()->Bind();
				(*m_ParticleStorage)[it.index].GetMaterial()->Set("u_Time", dt);
			}
			(*m_ParticleStorage)[it.index].Bind();
		}
	}

	void ParticleSystem2D::Add(Entity entity)
	{
		XYZ_LOG_INFO("Entity with id ", entity, " added");

		Component component;
		component.index = ECSManager::Get()->GetComponentIndex<ParticleEffect2D>(entity);
		component.entity = entity;

		auto it = std::lower_bound(m_Components.begin(), m_Components.end(), component, [](const Component& a, const Component& b) {
			
			auto storage = ECSManager::Get()->GetComponentStorage<ParticleEffect2D>();
			return (*storage)[a.index].GetMaterial()->GetSortKey() > (*storage)[b.index].GetMaterial()->GetSortKey();
		});

		m_Components.insert(it, component);
	}

	void ParticleSystem2D::Remove(Entity entity)
	{
		auto it = std::find(m_Components.begin(), m_Components.end(), entity);
		if (it != m_Components.end())
		{
			XYZ_LOG_INFO("Entity with id ", entity, " removed");
			it = m_Components.end() - 1;
			m_Components.erase(m_Components.end() - 1);
		}
	}

	bool ParticleSystem2D::Contains(Entity entity)
	{
		auto it = std::find(m_Components.begin(), m_Components.end(), entity);
		if (it != m_Components.end())
			return true;
		return false;
	}
}