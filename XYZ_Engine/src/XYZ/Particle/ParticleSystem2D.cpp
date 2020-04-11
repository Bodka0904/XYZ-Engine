#include "stdafx.h"
#include "ParticleSystem2D.h"
#include "XYZ/Renderer/Renderable2D.h"

namespace XYZ {
	ParticleSystem2D::ParticleSystem2D()
	{
		m_Signature.set(XYZ::ECSManager::Get()->GetComponentType<XYZ::ParticleEffect2D>());
	}

	void ParticleSystem2D::Update(float dt)
	{
		int16_t currentKey = 0;
		for (size_t i = 0; i < m_Components.size(); ++i)
		{
			int16_t newKey = (int16_t)m_Components[i].effect->GetMaterial()->GetSortKey();
			if (currentKey != newKey)
			{
				currentKey = newKey;
				m_Components[i].effect->GetMaterial()->Bind();
				m_Components[i].effect->GetMaterial()->Set("u_Time", dt);
			}
			m_Components[i].effect->Bind();
		}
	}

	void ParticleSystem2D::Add(Entity entity)
	{
		XYZ_LOG_INFO("Entity with id ", entity, " added");

		Component component;
		component.effect = &ECSManager::Get()->GetComponent<ParticleEffect2D>(entity);
		component.entity = entity;

		auto it = std::lower_bound(m_Components.begin(), m_Components.end(), component, [](const Component& a, const Component& b) {
			return a.effect->GetMaterial()->GetSortKey() > b.effect->GetMaterial()->GetSortKey();
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