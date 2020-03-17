#pragma once

#include "VertexArray.h"

#include <glm/glm.hpp>
#include <memory>
#include <functional>

namespace XYZ {
	class APIContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

		static std::unique_ptr<APIContext> Create(void* window);
	};
}