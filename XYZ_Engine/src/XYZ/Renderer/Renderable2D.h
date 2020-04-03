#pragma once
#include "Material.h"

#include <glm/glm.hpp>

namespace XYZ {

	struct Renderable2D
	{
		std::shared_ptr<Material> material;
		glm::vec4 color = glm::vec4(0);
		glm::vec4 texCoord = glm::vec4(0); // (x = left, y = bottom, z = right, w = top)
		glm::vec3 position = glm::vec3(0);
		glm::vec2 size = glm::vec2(0);
		float rotation = 0.0f;
		bool visible = true;
		int textureID = 0;
	};
}