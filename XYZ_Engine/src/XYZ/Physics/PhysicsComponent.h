#pragma once
#include "XYZ/ECS/Component.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace XYZ {
	template<typename T>
	using vector2D = std::vector<std::vector<T>>;

	class Transform2D : public Type<Transform2D>
	{
	public:
		Transform2D(const glm::vec3& pos, const glm::vec2& size = glm::vec2(1), float rot = 0.0f);
		Transform2D() = default;

		void SetParent(Transform2D* parent);

		// Does not set m_Updated to true
		const glm::vec3& GetPosition() const;
		const glm::vec2& GetSize() const;
		float GetRotation() const;

		// Sets m_Updated to true
		glm::vec3& GetPosition();
		glm::vec2& GetSize();
		float& GetRotation();

		const glm::mat4& GetTransformation() const;
	private:
		void recalculateTransform() const;

	private:
		// It is required to be mutable , we use GetTransformation in functions 
		// that are not allowed to modify passed references
		mutable glm::mat4 m_Transformation = glm::mat4(1);


		glm::vec3 m_Position = glm::vec3(0);
		glm::vec2 m_Size = glm::vec2(1);
		float m_Rotation = 0.0f;

		// It is required to be mutable , we use GetTransformation in functions 
		// that are not allowed to modify passed references
		mutable bool m_Updated = false;

		Transform2D* m_Parent = nullptr;
		Transform2D* m_FirstChild = nullptr;
		Transform2D* m_NextSibling = nullptr;
	};




	struct RigidBody2D : public Type<RigidBody2D>
	{
		RigidBody2D(const glm::vec2& velocity)
			: Velocity(velocity), BounceValue(0)
		{}
		RigidBody2D() = default;

		glm::vec2 Velocity = glm::vec2(0);
		int BounceValue = 0;
	};

	struct GridBody : public Type<GridBody>
	{
		GridBody(int row, int col, int width, int height)
			: Row(row), Col(col), Width(width), Height(height), NextRow(0), NextCol(0)
		{}

		GridBody() = default;

		int Row = 0;
		int Col = 0;
		int Width = 0;
		int Height = 0;
		int NextRow = 0;
		int NextCol = 0;

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
		InterpolatedMovement() = default;

		glm::vec2 Distance = glm::vec2(0);
		glm::vec2 Velocity = glm::vec2(0);
		bool InProgress = false;
	};

	struct CollisionComponent : public Type<CollisionComponent>
	{
		CollisionComponent(int32_t layer, int32_t collisionLayers)
			: Layer(layer), CollisionLayers(collisionLayers), CurrentCollisions(0)
		{}

		CollisionComponent() = default;

		int32_t Layer = 0;

		// layers that might collide with layer
		int32_t CollisionLayers = 0;

		// current collisions
		int32_t CurrentCollisions = 0;
	};


	struct RealGridBody : public Type<RealGridBody>
	{
		RealGridBody(float left, float right, float bottom, float top)
			: Left(left), Right(right), Bottom(bottom), Top(top)
		{}

		RealGridBody() = default;

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

		float Left = 0.0f;
		float Right = 0.0f;
		float Bottom = 0.0f;
		float Top = 0.0f;
	};

}


