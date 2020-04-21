#pragma once
#include <glm/glm.hpp>
#include "XYZ/ECS/Component.h"

namespace XYZ {

	struct Transform2D : public Type<Transform2D>
	{
		glm::vec2 pos = glm::vec2(0);
		glm::vec2 rot = glm::vec2(0);
		glm::vec2 scale = glm::vec2(1);
	};

	struct RigidBody2D : public Type<RigidBody2D>
	{
		glm::vec2 velocity = glm::vec2(0);
	};

	
	struct GridPosition : public Type<GridPosition>
	{
		GridPosition(int Row = 0,int Col = 0)
			: row(Row),col(Col)
		{}
		int row = 0;
		int col = 0;


		bool operator ==(const GridPosition& other) const
		{
			return (col == other.col && row == other.row);
		}

		bool operator !=(const GridPosition& other)
		{
			return (col != other.col || row != other.row);
		}

		GridPosition operator+(const RigidBody2D& other)
		{
			GridPosition pos = *this;
			pos.col += other.velocity.x;
			pos.row += other.velocity.y;
			return pos;
		}
		GridPosition& operator += (const RigidBody2D& other)
		{
			col += other.velocity.x;
			row += other.velocity.y;
			return *this;
		}
	};

	struct InterpolatedMovement : public Type<GridPosition>
	{
		float currentTime = 0.0f;
		float length = 0.5f;
		bool inProgress = false;

		glm::vec2 velocity;
	};

	struct Collision : public Type<Collision>
	{

	};

	struct LayerMask : public Type<LayerMask>
	{
		LayerMask(int layerBit = 0)
		{
			XYZ_ASSERT(layerBit < 32, "Max number of layers is 32");
			mask = 0;
			mask |= 1 << layerBit;
		}
		int32_t mask;
	};
}


