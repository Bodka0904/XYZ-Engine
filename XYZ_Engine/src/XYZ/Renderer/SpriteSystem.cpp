#include "stdafx.h"
#include "SpriteSystem.h"


namespace XYZ {
	SpriteSystem::SpriteSystem()
	{	
		m_Signature.set(ECSManager::Get()->GetComponentType<SpriteAnimation>());
		m_Signature.set(ECSManager::Get()->GetComponentType<Sprite>());
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
		SpriteComponent component;
		component.animation = &ECSManager::Get()->GetComponent<SpriteAnimation>(entity);
		component.sprite = &ECSManager::Get()->GetComponent<Sprite>(entity);
		component.entity = entity;
		m_Components.push_back(component);
	}
	void SpriteSystem::Remove(Entity entity)
	{
		std::sort(m_Components.begin(), m_Components.end(), Compare());

		int position = binarySearch(0, (int)m_Components.size() - 1, entity);
		if (position != -1 && !m_Components.empty())
		{
			XYZ_LOG_INFO("Entity with ID ", entity, " removed");
			m_Components[position] = m_Components[m_Components.size() - 1];
			m_Components.erase(m_Components.end() - 1);
		}
	}
	int SpriteSystem::binarySearch(int start, int end, Entity entity)
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