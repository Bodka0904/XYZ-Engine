#pragma once
#include <memory>
#include <inttypes.h>

#include "Shader.h"
#include "Camera.h"
#include "VertexArray.h"
#include "RendererAPI.h"


namespace XYZ {
	struct RendererSubmitData
	{
		const std::shared_ptr<Shader> m_Shader;
		const std::shared_ptr<VertexArray> m_VertexArray;

		RendererSubmitData(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray> vertexArray) : m_Shader(shader), m_VertexArray(vertexArray) {};
	};

	class Renderer
	{
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};
	public:
		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		static void BeginScene(Camera& camera);
		static void EndScene();

		static void Submit(RendererSubmitData& data);
	private:
		static SceneData* m_SceneData;
	};

}