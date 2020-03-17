#pragma once
#include "XYZ/ECS/ECSManager.h"
#include "Components.h"



namespace XYZ {
	class PhysicsSystem : public System
	{
	public:
		PhysicsSystem();
		virtual void Update(float dt);
		virtual void Add(Entity entity) override;
		virtual void Remove(Entity entity) override;
	private:
		int binarySearch(int start, int end, Entity entity);
	private:
		struct PhysicsComponent
		{
			Transform2D* transform;
			RigidBody2D* rigidbody;
			Entity entity;
		};

		struct Compare
		{
			bool operator()(const PhysicsComponent& a, const PhysicsComponent& b)
			{
				return a.entity < b.entity;
			}
		};

		float m_Gravity = 10.0f;
		std::vector<PhysicsComponent> m_Components;
	};

}