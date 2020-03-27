#include "stdafx.h"
#include "MaterialManager.h"



namespace XYZ {
	MaterialManager	MaterialManager::s_Instance;


	std::shared_ptr<Material> MaterialManager::GetMaterial(int16_t id)
	{
		return findMaterial(id);
	}

	MaterialManager::MaterialManager()
		: m_Sorted(false)
	{
		for (int16_t id = 0; id < sc_MaxNumberOfMaterials; ++id)
		{
			m_AvailableIDs.push(id);
		}
	}
	int16_t MaterialManager::registerMaterial(std::shared_ptr<Material> material)
	{
		XYZ_ASSERT(m_MaterialsInExistence < sc_MaxNumberOfMaterials, "Too many materials in existence");
		int16_t id = m_AvailableIDs.front();
		m_AvailableIDs.pop();
		m_MaterialsInExistence++;
		m_Materials.push_back(KeyMaterial{ material,id });
		m_Sorted = false;
		return id;
	}
	void MaterialManager::destroyMaterial(int16_t id)
	{
		m_AvailableIDs.push(id);
		m_MaterialsInExistence--;
	}
	std::shared_ptr<Material> MaterialManager::findMaterial(int16_t id)
	{
		if (!m_Sorted)
		{
			std::sort(m_Materials.begin(), m_Materials.end(), [](const KeyMaterial& a, const KeyMaterial& b) {
				return a.key < b.key;
			});
			m_Sorted = true;
		}
		
		int index = binarySearch(0, m_Materials.size()-1, id);
		XYZ_ASSERT(index != -1, "Requesting not existing material");

		return m_Materials[(size_t)index].material;
	}
	int MaterialManager::binarySearch(int start, int end, int16_t key)
	{
		if (end >= start)
		{
			int mid = start + (end - start) / 2;
			if (m_Materials[mid].key == key)
				return mid;

			if (m_Materials[mid].key > key)
				return binarySearch(start, mid - 1, key);


			return binarySearch(mid + 1, end, key);
		}
		return -1;
	}
}