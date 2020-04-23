#pragma once
#include "Renderable2D.h"
#include "VertexArray.h"

namespace XYZ {

	template <typename T,typename _Comparator>
	using MaterialGroup = std::map<std::shared_ptr<Material>, std::vector<T>,_Comparator>;

	/**
	* @class SortingGroup
	* @brief Sort elements dependent on specfied Comparator 
	* @tparam T		Comparator
	*/
	template <typename _Type ,typename _Comparator>
	class SortingGroup
	{
	private:
		struct MaterialComparator
		{
			bool operator()(const std::shared_ptr<Material>& a, const std::shared_ptr<Material>& b) const
			{
				return a->GetSortKey() < b->GetSortKey();
			}
		};
	public:
		/**
		* Insert index to the vector , keeps it sorted
		* @param[in] index		index of the element
		*/
		void Add(std::shared_ptr<Material> material,_Type index)
		{
			auto it = std::lower_bound(m_Group[material].begin(), m_Group[material].end(), index, _Comparator());
			m_Group[material].insert(it, index);	
		}
		/**
		* Remove index from the vector, in linear time
		* @param[in] index
		*/
		void Remove(std::shared_ptr<Material> material,_Type index)
		{
			auto it = std::find(m_Group[material].begin(), m_Group[material].end(), index);
			if (it == m_Group[material].end())
				XYZ_ASSERT(false, "Attempting to remove not existing element");

			m_Group[material].erase(it);
		}

		/**
		* @return material group
		*/
		const MaterialGroup<_Type,MaterialComparator>& GetGroup() { return m_Group; }


	
	private:
		MaterialGroup<_Type, MaterialComparator> m_Group;
	};

}