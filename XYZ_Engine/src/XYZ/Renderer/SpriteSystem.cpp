#include "stdafx.h"
#include "SpriteSystem.h"


namespace XYZ {
	SpriteSystem::SpriteSystem()
	{	
		m_Signature.set(ECSManager::Get()->GetComponentType<SpriteAnimation>());
		m_Signature.set(ECSManager::Get()->GetComponentType<Renderable2D>());
	}
	void SpriteSystem::Update(float dt)
	{
		for (size_t i = 0; i < m_Components.size(); ++i)
		{
			m_Components[i].animation->Update(dt,m_Components[i].sprite);
		}
	}
	void SpriteSystem::Add(Entity entity)
	{
		XYZ_LOG_INFO("Entity with ID ", entity, " added");
		Component component;
		component.animation = &ECSManager::Get()->GetComponent<SpriteAnimation>(entity);
		component.sprite = &ECSManager::Get()->GetComponent<Renderable2D>(entity);
		component.entity = entity;
		m_Components.push_back(component);
	}
	void SpriteSystem::Remove(Entity entity)
	{		
		auto it = std::find(m_Components.begin(), m_Components.end(), entity);
		if (it != m_Components.end())
		{
			XYZ_LOG_INFO("Entity with id ", entity, " removed");
			it = m_Components.end() - 1;
			m_Components.erase(m_Components.end() - 1);
		}
	}
	bool SpriteSystem::Contains(Entity entity)
	{	
		auto it = std::find(m_Components.begin(), m_Components.end(), entity);
		if (it != m_Components.end())
			return true;
		return false;
	}
}