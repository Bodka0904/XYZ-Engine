#include "stdafx.h"
#include "PhysicsComponent.h"




namespace XYZ {
	Transform2D::Transform2D(const glm::vec3& pos, const glm::vec2& size, float rot)
		:
		m_Position(pos),
		m_Size(size),
		m_Rotation(rot)
	{
		recalculateTransform();
	}
	void Transform2D::SetParent(Transform2D* parent)
	{
		// Set parent
		m_Parent = parent;

		// Set parent first child to next sibling
		m_NextSibling = parent->m_FirstChild;
		
		// Take first place in parent children linked list
		parent->m_FirstChild = this;
	}
	const glm::mat4& Transform2D::GetTransformation() const
	{
		if (m_Updated)
			recalculateTransform();
		return m_Transformation;
	}
	const glm::vec3& Transform2D::GetPosition() const
	{
		return m_Position;
	}

	const glm::vec2& Transform2D::GetSize() const
	{
		return m_Size;
	}

	float Transform2D::GetRotation() const
	{
		return m_Rotation;
	}

	glm::vec3& Transform2D::GetPosition()
	{
		m_Updated = true;
		return m_Position;
	}

	glm::vec2& Transform2D::GetSize()
	{
		m_Updated = true;
		return m_Size;
	}

	float& Transform2D::GetRotation()
	{
		m_Updated = true;
		return m_Rotation;
	}

	void Transform2D::recalculateTransform() const
	{
		glm::mat4 posMatrix = glm::translate(m_Position);
		glm::mat4 rotMatrix = glm::rotate(m_Rotation, glm::vec3(0, 0, 1));
		glm::mat4 scaleMatrix = glm::scale(glm::vec3(m_Size, 1));
		m_Transformation = posMatrix * rotMatrix * scaleMatrix;
		m_Updated = false;
	}

}