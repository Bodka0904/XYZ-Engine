#include "stdafx.h"
#include "ParticleSystem2D.h"
#include "XYZ/Renderer/Renderable2D.h"
#include "XYZ/Renderer/RenderCommand.h"
#include "XYZ/Renderer/Renderer2D.h"
#include "XYZ/Renderer/RenderCommandQueue.h"

namespace XYZ {
	ParticleSystem2D::ParticleSystem2D()
	{
		m_Signature.set(XYZ::ECSManager::Get().GetComponentType<XYZ::ParticleEffect2D>());	
	}

	void ParticleSystem2D::Update(float dt)
	{
		if (!m_Sorted)
			std::sort(m_Components.begin(), m_Components.end(), [](Component& a, Component& b) {
			return a.Effect.Get().GetMaterial()->GetSortKey() < b.Effect.Get().GetMaterial()->GetSortKey();
		});
		int16_t currentKey = -1;
		for (auto &it : m_Components)
		{
			int16_t newKey = it.Effect.Get().GetMaterial()->GetSortKey();
			if (currentKey != newKey)
			{
				currentKey = newKey;
				it.Effect.Get().GetMaterial()->Set("u_Time", dt);
				it.Effect.Get().GetMaterial()->Bind();
			}

			it.Effect.Get().Update(dt);
			it.Effect.Get().Render();		
		}
	}

	void ParticleSystem2D::Add(Entity entity)
	{
		XYZ_LOG_INFO("Entity with id ", entity, " added");

		Component component;
		component.ActiveComponent = ECSManager::Get().GetComponent<ActiveComponent>(entity);
		component.Effect = ECSManager::Get().GetComponent<ParticleEffect2D>(entity);
		
		m_Components.push_back(component);
		m_Sorted = false;
	}

	void ParticleSystem2D::Remove(Entity entity)
	{
		auto it = std::find(m_Components.begin(), m_Components.end(), entity);
		if (it != m_Components.end())
		{
			XYZ_LOG_INFO("Entity with id ", entity, " removed");
			*it = std::move(m_Components.back());
			m_Components.pop_back();
			m_Sorted = false;
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