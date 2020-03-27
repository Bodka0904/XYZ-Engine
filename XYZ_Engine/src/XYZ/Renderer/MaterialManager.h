#pragma once
#include "Material.h"


#include <queue>

namespace XYZ {

	class MaterialManager
	{
		friend class Material;
	public:
		MaterialManager(const MaterialManager&) = delete;

		// temporary
		std::shared_ptr<Material> GetMaterial(int16_t id);
		
		
		static MaterialManager& Get() { return s_Instance; }
	private:
		MaterialManager();
		int16_t registerMaterial(std::shared_ptr<Material> material);
		void    destroyMaterial(int16_t id);
		std::shared_ptr<Material> findMaterial(int16_t id);

		int binarySearch(int start, int end, int16_t key);
	private:
		int16_t m_MaterialsInExistence = 0;
		std::queue<int16_t> m_AvailableIDs{};

		// Temporary untill we have asset manager
		struct KeyMaterial
		{
			std::shared_ptr<Material> material;
			int16_t key;
		};
		std::vector<KeyMaterial> m_Materials;
		bool m_Sorted;

		static MaterialManager s_Instance;
		static constexpr int16_t sc_MaxNumberOfMaterials = 2 ^ (sizeof(int16_t) * 8);
	};
}