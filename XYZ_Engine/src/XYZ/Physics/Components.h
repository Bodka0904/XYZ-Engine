#pragma once
#include <glm/glm.hpp>
#include "XYZ/ECS/Component.h"

namespace XYZ {
	template<typename T>
	using vector2D = std::vector<std::vector<T>>;

	struct Transform2D : public Type<Transform2D>
	{
		Transform2D(const glm::vec3& pos, const glm::vec2& size = glm::vec2(1), float rot = 0.0f)
			:
			Position(pos),Size(size),Rotation(rot)
		{}
		glm::vec3 Position = glm::vec3(0);
		glm::vec2 Size = glm::vec2(1);
		float Rotation = 0.0f;
	};

	struct RigidBody2D : public Type<RigidBody2D>
	{
		RigidBody2D(const glm::vec2& velocity)
			: Velocity(velocity),BounceValue(0)
		{}
		glm::vec2 Velocity;
		int BounceValue;
	};
	
	struct GridBody : public Type<GridBody>
	{
		GridBody(int row,int col,int width, int height)
			: Row(row),Col(col),Width(width),Height(height),NextRow(0),NextCol(0)
		{}
		int Row;
		int Col;
		int Width;
		int Height;
		int NextRow;
		int NextCol;

		bool operator ==(const GridBody& other) const
		{
			return (Col == other.Col && Row == other.Row);
		}

		bool operator !=(const GridBody& other)
		{
			return (Col != other.Col || Row != other.Row);
		}
	};

	struct InterpolatedMovement : public Type<InterpolatedMovement>
	{
		InterpolatedMovement(const glm::vec2& velocity)
			: Distance(glm::vec2(0)), Velocity(velocity), InProgress(false)
		{}
		glm::vec2 Distance;
		glm::vec2 Velocity;
		bool InProgress;
	};

	struct CollisionComponent : public Type<CollisionComponent>
	{
		CollisionComponent(int32_t layer, int32_t collisionLayers)
			: Layer(layer), CollisionLayers(collisionLayers), CurrentCollisions(0)
		{}
		int32_t Layer;

		// layers that might collide with layer
		int32_t CollisionLayers;

		// current collisions
		int32_t CurrentCollisions;
	};


	struct RealGridBody : public Type<RealGridBody>
	{
		RealGridBody(float left, float right, float bottom, float top)
			: Left(left),Right(right),Bottom(bottom),Top(top)
		{}

		bool Collide(const RealGridBody& other)
		{
			if (Left >= other.Right || other.Left >= Right)
			{
				return false;
			}
			if (Top <= other.Bottom || other.Top <= Bottom)
			{
				return false;
			}
			return true;
		}

		void Move(const glm::vec2& pos)
		{
			Left += pos.x;
			Right += pos.x;
			Top += pos.y;
			Bottom += pos.y;
		}

		float Left;
		float Right;
		float Bottom;
		float Top;
	};

}


