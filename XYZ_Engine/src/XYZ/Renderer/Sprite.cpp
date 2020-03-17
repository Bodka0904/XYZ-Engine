#include "stdafx.h"
#include "Sprite.h"


namespace XYZ {
	Sprite::Sprite(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const glm::vec4& texCoords)
		: Renderable2D(position,size,color)
	{
		SetTexCoord(texCoords);
	}
	
}