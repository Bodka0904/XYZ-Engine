#pragma once
#include <glm/glm.hpp>

namespace XYZ {

	struct Vertex
	{
		glm::vec4 color;
		glm::vec3 position;
		glm::vec2 texCoord;
		float	  textureID;
	};

	class Renderable2D
	{
	public:
		Renderable2D(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

		void SetPosition(const glm::vec3& position);
		void SetTextureID(int id);
		void SetTexCoord(const glm::vec4& texCoords);
		void SetVisible(bool visible) { m_IsVisible = visible; }
		
		const Vertex* GetVertices() { return &m_Vertices[0]; }
	protected:
		Vertex m_Vertices[4];
		glm::vec2 m_Size;
		float  m_Rotation;
		bool   m_IsVisible;
	};
}