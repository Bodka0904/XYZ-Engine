#pragma once

namespace XYZ {
	template <typename T,typename Function>
	class Tree;

	template <typename T,typename Function>
	class Node
	{	
		friend class Tree<T, Function>;
	public:
		Node(const T& data)
			:
			m_Data(data)
		{}
		
		bool HasChild()
		{
			return m_FirstChild != sc_NullIndex;
		}

		uint16_t GetParentIndex() const { return m_Parent; }

		const T& GetData() const { return m_Data; }
		
		T& GetData() { return m_Data; }

	private:
		void setIndex(uint16_t index)
		{
			m_Index = index;
		}
		void setParent(uint16_t parent, std::vector<Node<T, Function>>& vector)
		{
			// Assing parent
			m_Parent = parent;

			// Assign to first child previous sibling this
			uint16_t firstChildIndex = vector[parent].m_FirstChild;
			if (firstChildIndex != sc_NullIndex)
			{
				vector[firstChildIndex].m_PreviousSibling = m_Index;
			}
			// Set as next sibling current first child
			m_NextSibling = vector[parent].m_FirstChild;

			// Set as first child this
			vector[parent].m_FirstChild = m_Index;
		}

		void detachFromTree(std::vector<Node<T, Function>>& vector)
		{
			// If previous sibling exist assign to its next sibling this next sibling
			if (vector[m_Index].m_PreviousSibling != sc_NullIndex)
			{
				uint16_t previous = vector[m_Index].m_PreviousSibling;
				vector[previous].m_NextSibling = vector[m_Index].m_NextSibling;
			}
			else
			{
				// If previous sibling does not exist assign this next sibling directly as first child of parent
				vector[m_Parent].m_FirstChild = vector[m_Index].m_NextSibling;
			}
			m_Parent = sc_NullIndex;
			m_NextSibling = sc_NullIndex;
		}

		void reconnectToTree(std::vector<Node<T, Function>>& vector, uint16_t newIndex)
		{
			// Reconnect with previous sibling
			if (vector[m_Index].m_PreviousSibling != sc_NullIndex)
			{
				uint16_t previous = vector[m_Index].m_PreviousSibling;
				vector[previous].m_NextSibling = newIndex;
			}
			else
			{
				// If not previous sibling connect directly to parent
				vector[m_Parent].m_FirstChild = newIndex;
			}
			// Reconnect with next sibling
			if (vector[m_Index].m_NextSibling != sc_NullIndex)
			{
				uint16_t next = vector[m_Index].m_NextSibling;
				vector[next].m_PreviousSibling = newIndex;
			}
			
			// Reconnect with children
			uint16_t child = vector[m_Index].m_FirstChild;
			while (child != sc_NullIndex)
			{
				vector[child].m_Parent = newIndex;
				child = vector[child].m_NextSibling;
			}

			m_Index = newIndex;
		}


	private:
		static constexpr uint16_t sc_NullIndex = 65535;

	private:
		T m_Data;
		uint16_t m_Index = sc_NullIndex;
		uint16_t m_Parent = sc_NullIndex;
		uint16_t m_FirstChild = sc_NullIndex;
		uint16_t m_PreviousSibling = sc_NullIndex;
		uint16_t m_NextSibling = sc_NullIndex;
	};


	template <typename T, typename Function>
	class Tree
	{
	public:
		void Reserve(size_t count)
		{
			m_Data.reserve(count);
		}
		uint16_t InsertNode(const Node<T,Function>& node)
		{
			XYZ_ASSERT(m_Data.size() < sc_MaxNumberOfElements, "Maximum number of elements in tree is ", sc_MaxNumberOfElements);
			m_Data.push_back(node);
			uint16_t index = m_Data.size() - 1;
			m_Data[index].setIndex(index);
			return index;
		}
		void SetRoot(uint16_t root)
		{
			XYZ_ASSERT(root < m_Data.size(), "Index out of range");
			m_Root = root;
		}
		void SetParent(uint16_t parent, uint16_t child)
		{
			XYZ_ASSERT(parent < m_Data.size() && child < m_Data.size(), "Index out of range");
			m_Data[child].setParent(parent,m_Data);
		}
		void DeleteNode(uint16_t index)
		{
			XYZ_ASSERT(index < m_Data.size(), "Index out of range");
			
			// Detach from tree
			m_Data[index].detachFromTree(m_Data);

			// Reconnect the element that was last in the container with index of deleted element
			uint16_t last = m_Data.size() - 1;
			m_Data[last].reconnectToTree(m_Data, index);

			auto it = m_Data.begin() + index;
			// Move last element to the place of old one
			*it = std::move(m_Data.back());
			// Pops last element
			m_Data.pop_back();	
		}

		void Propagate()
		{
			propagate(m_Root);
		}
		
	private:
		void propagate(uint16_t index)
		{
			while (index != Node<T, Function>::sc_NullIndex)
			{
				Function()(m_Data, index);
				if (m_Data[index].HasChild())
				{
					uint16_t firstChild = m_Data[index].m_FirstChild;
					propagateRecursive(firstChild);
				}
				index = m_Data[index].m_NextSibling;
			}
		}

		void deleteNode(uint16_t index)
		{
			while (index != Node<T, Function>::sc_NullIndex)
			{
				// Detach from tree
				m_Data[index].detachFromTree(m_Data);

				uint16_t last = m_Data.size() - 1;
				m_Data[last].reconnectToTree(m_Data, index);

				auto it = m_Data.begin() + index;
				// Move last element to the place of old one
				*it = std::move(m_Data.back());
				// Pops last element
				m_Data.pop_back();
			}
		}

	private:
		// Reference to vector of nodes
		std::vector<Node<T,Function>> m_Data;

		// Root by default null
		uint16_t m_Root = sc_MaxNumberOfElements;
		// Last value is used as null indicator
		static constexpr uint16_t sc_MaxNumberOfElements = (2 ^ 16) - 1;
	};
}