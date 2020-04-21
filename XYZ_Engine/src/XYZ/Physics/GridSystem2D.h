#pragma once
#include "XYZ/ECS/ECSManager.h"
#include "Components.h"

namespace XYZ {
	template<typename T>
	using vector2D = std::vector<std::vector<T>>;

	class GridSystem2D : public System
	{
	public:
		GridSystem2D(int rows,int cols,int cellSize);

		void Update(float dt);
		virtual void Add(Entity entity) override;
		virtual void Remove(Entity entity) override;
		virtual bool Contains(Entity entity) override;
		
	private:
		struct Component : public System::Component
		{
			Transform2D* transform;
			RigidBody2D* rigidbody;
		};

		int m_Rows;
		int m_Cols;
		int m_CellSize;

		vector2D<Component> m_Cells;
	};
}