#pragma once
#include "XYZ/ECS/ECSManager.h"
#include "XYZ/Renderer/VertexArray.h"
#include "XYZ/Renderer/Material.h"
#include "XYZ/Renderer/Renderable2D.h"
#include "XYZ/Renderer/OrthoCamera.h"


#include "XYZ/Renderer/RenderCommandQueue.h"
namespace XYZ {
	class RendererSystem2D : public System
	{
	public:	
		RendererSystem2D();
		// In future might be removed
		virtual void Add(Entity entity) override;
		virtual void Remove(Entity entity) override;

		/* In future it will probably take whole scene object instead,
		which would be provided by scene manager/system. Now maximal number of renderables is 
		locked to maximal number of entities */
		void BeginScene(const OrthoCamera& camera);

		void EndScene();
		void Flush();
		virtual bool Contains(Entity entity);
	private:
		struct Vertex2D
		{
			glm::vec4 color;
			glm::vec3 position;
			glm::vec2 texCoord;
			float	  textureID;
		};

		struct RenderBatch
		{
			std::shared_ptr<VertexArray> vao;
			std::shared_ptr<Material> material;

			uint32_t indexCount = 0;
			Vertex2D* bufferBase = nullptr;
			Vertex2D* bufferPtr = nullptr;

			void Submit(const Renderable2D& renderable);
		};

		struct Component : public System::Component
		{
			Renderable2D* renderComponent;
		};
		std::vector<Component> m_Components;


		std::vector<RenderBatch> m_Batches;

		const uint32_t c_MaxQuads = 10000;
		const uint32_t c_MaxVertices = c_MaxQuads * 4;
		const uint32_t c_MaxIndices = c_MaxQuads * 6;
	};
}

