#pragma once
#include <glm/glm.hpp>

namespace XYZ {

	struct Transform2D
	{
		glm::vec2 pos = glm::vec2(0);
		glm::vec2 scale = glm::vec2(1);
		float rot = 0.0f;
	};

	struct RigidBody2D
	{
		glm::vec2 velocity = glm::vec2(0);
	};
}