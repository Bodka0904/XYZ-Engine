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
		if (!m_KeySorted)
		{
			std::sort(m_Components.begin(), m_Components.end(), [](const Component& a, const Component& b) {
				return a.effect->GetMaterial()->GetSortKey() > b.effect->GetMaterial()->GetSortKey();
			});
			m_KeySorted = true;
			m_EntitySorted = false;
		}


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
		m_Components.push_back(component);

		m_EntitySorted = false;
		m_KeySorted = false;
	}

	void ParticleSystem2D::Remove(Entity entity)
	{
		m_KeySorted = false;
		if (!m_EntitySorted)
		{
			std::sort(m_Components.begin(), m_Components.end(), Component());
			m_EntitySorted = true;
		}
		int position = binarySearch(0, (int)m_Components.size() - 1, entity, m_Components);
		if (position != -1 && !m_Components.empty())
		{
			XYZ_LOG_INFO("Entity with id ", entity, " removed");
			m_Components[position] = m_Components[m_Components.size() - 1];
			m_Components.erase(m_Components.end() - 1);

			m_EntitySorted = false;
		}
	}

	bool ParticleSystem2D::Contains(Entity entity)
	{
		m_KeySorted = false;
		if (!m_EntitySorted)
		{
			std::sort(m_Components.begin(), m_Components.end(), Component());
			m_EntitySorted = true;
		}
		int position = binarySearch(0, (int)m_Components.size() - 1, entity, m_Components);
		if (position == -1 || m_Components.empty())
			return false;
		
		return true;
	}
}