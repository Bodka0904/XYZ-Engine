#pragma once

#include <glm/glm.hpp>


namespace XYZ {
	class OrthoCamera
	{
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position;
		float m_Rotation = 0.0f;
	private:
		void RecalculateViewMatrix();
	public:
		OrthoCamera(float left, float right, float bottom, float top);

		const glm::vec3& GetPosition() const { return m_Position; }
		inline void SetPosition(const glm::vec3& pos) { m_Position = pos; RecalculateViewMatrix(); }

		float GetRotation() const { return m_Rotation; }
		void SetRotation(float r) { m_Rotation = r; RecalculateViewMatrix(); }

		void SetProjection(float left, float right, float bottom, float top);

		inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		inline const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
	};

}