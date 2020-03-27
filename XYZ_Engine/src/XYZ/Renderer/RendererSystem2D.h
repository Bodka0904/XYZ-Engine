#pragma once
#include "XYZ/ECS/ECSManager.h"
#include "XYZ/Renderer/VertexArray.h"

namespace XYZ {
	class RendererSystem2D : public System
	{
	public:
		virtual void Add(Entity entity);
		virtual void Remove(Entity entity);


		void Submit();
	private:
		std::shared_ptr<VertexArray> m_VAO;

		const uint32_t c_MaxQuads = 10000;
		const uint32_t c_MaxVertices = c_MaxQuads * 4;
		const uint32_t c_MaxIndices = c_MaxQuads * 6;
	};
}