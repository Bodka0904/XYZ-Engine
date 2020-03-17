#pragma once
#include "Renderable2D.h"
namespace XYZ {
	class Sprite : public Renderable2D
	{
	public:
		Sprite(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const glm::vec4& texCoords);

	};
}