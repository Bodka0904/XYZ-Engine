#pragma once
#include "Renderable2D.h"
#include "VertexArray.h"

namespace XYZ {

	typedef std::map<std::shared_ptr<Material>, std::vector<Renderable2D*>> RenderablesGroup;

	template <typename T>
	class SortingGroup
	{
	public:
		void AddRenderable(Renderable2D* renderable)
		{
			auto material = renderable->material;
			auto it = std::lower_bound(m_Renderables[material].begin(), m_Renderables[material].end(), renderable, T());
			m_Renderables[material].insert(it, renderable);
		}
		void RemoveRenderable(const Renderable2D* renderable)
		{
			auto material = renderable->material;
			auto it = std::find(m_Renderables[material].begin(), m_Renderables[material].end(), renderable);
			if (it == m_Renderables[material].end())
				XYZ_ASSERT(false, "Attempting to remove not existing renderable");

			m_Renderables[material].erase(it);
		}
		const RenderablesGroup& GetRenderables() { return m_Renderables; }

	private:
		RenderablesGroup m_Renderables;
	};

}