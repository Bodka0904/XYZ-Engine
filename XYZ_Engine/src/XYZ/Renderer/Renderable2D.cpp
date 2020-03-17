#include "stdafx.h"
#include "Renderable2D.h"


namespace XYZ {
	Renderable2D::Renderable2D(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
		: m_Rotation(0),m_IsVisible(true),m_Size(size)
	{
		m_Vertices[0].color = color;
		m_Vertices[1].color = color;
		m_Vertices[2].color = color;
		m_Vertices[3].color = color;

		m_Vertices[0].position = glm::vec3(position.x - size.x / 2.0f, position.y - size.y / 2.0f, position.z);
		m_Vertices[1].position = glm::vec3(position.x + size.x / 2.0f, position.y - size.y / 2.0f, position.z);
		m_Vertices[2].position = glm::vec3(position.x + size.x / 2.0f, position.y + size.y / 2.0f, position.z);
		m_Vertices[3].position = glm::vec3(position.x - size.x / 2.0f, position.y + size.y / 2.0f, position.z);

		m_Vertices[0].texCoord = glm::vec2(0);
		m_Vertices[1].texCoord = glm::vec2(0);
		m_Vertices[2].texCoord = glm::vec2(0);
		m_Vertices[3].texCoord = glm::vec2(0);

		m_Vertices[0].textureID = 0;
		m_Vertices[1].textureID = 0;
		m_Vertices[2].textureID = 0;
		m_Vertices[3].textureID = 0;
	}
	void Renderable2D::SetPosition(const glm::vec3& position)
	{
		m_Vertices[0].position = glm::vec3(position.x - m_Size.x / 2.0f, position.y - m_Size.y / 2.0f, position.z);
		m_Vertices[1].position = glm::vec3(position.x + m_Size.x / 2.0f, position.y - m_Size.y / 2.0f, position.z);
		m_Vertices[2].position = glm::vec3(position.x + m_Size.x / 2.0f, position.y + m_Size.y / 2.0f, position.z);
		m_Vertices[3].position = glm::vec3(position.x - m_Size.x / 2.0f, position.y + m_Size.y / 2.0f, position.z);
	}
	void Renderable2D::SetTextureID(int id)
	{
		m_Vertices[0].textureID = id;
		m_Vertices[1].textureID = id;
		m_Vertices[2].textureID = id;
		m_Vertices[3].textureID = id;
	}
	void Renderable2D::SetTexCoord(const glm::vec4& texCoords)
	{
		m_Vertices[0].texCoord = glm::vec2(texCoords.x, texCoords.y);
		m_Vertices[1].texCoord = glm::vec2(texCoords.z, texCoords.y);
		m_Vertices[2].texCoord = glm::vec2(texCoords.z, texCoords.w);
		m_Vertices[3].texCoord = glm::vec2(texCoords.x, texCoords.w);
	}
}