#pragma once
/* Copied from 
https://stackoverflow.com/questions/41946007/efficient-and-well-explained-implementation-of-a-quadtree-for-2d-collision-det
*/
namespace XYZ {

	template <typename T>
	class FreeList
	{
	public:
		FreeList()
			: m_FirstFree(-1)
		{}
		FreeList(const FreeList<T>& other)
		{
			m_FirstFree = other.m_FirstFree;
			m_Data = other.m_Data;
		}
		FreeList<T>& operator=(const FreeList<T>& other)
		{
			m_FirstFree = other.m_FirstFree;
			m_Data = other.m_Data;
			return *this;
		}
		int Insert(const T& elem)
		{
			if (m_FirstFree != -1)
			{
				int index = m_FirstFree;
				m_FirstFree = m_Data[m_FirstFree].next;
				m_Data[index].element = elem;
				return index;
			}
			else
			{
				FreeElement el;
				el.element = elem;
				m_Data.push_back(el);
				return static_cast<int>(m_Data.size() - 1);
			}
		}
		void Erase(int index)
		{
			m_Data[index].next = m_FirstFree;
			m_FirstFree = index;
		}
		void Clear()
		{
			m_Data.clear();
			m_FirstFree = -1;
		}
		int Range() const
		{
			return static_cast<int>(m_Data.size());
		}

		T& operator[](int index)
		{
			return m_Data[index].element;
		}
		const T& operator[](int index) const
		{
			return m_Data[index].element;
		}

	private:
		union FreeElement
		{
			FreeElement()
				: element(T()),next(0)
			{}
			T element;
			int next;
		};
		std::vector<FreeElement> m_Data;
		int m_FirstFree;
	};
}