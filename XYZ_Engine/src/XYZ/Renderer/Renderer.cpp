#include "stdafx.h"
#include "Renderer.h"
#include "RenderCommand.h"
#include "Renderer2D.h"

namespace XYZ {
	Renderer::SceneData* Renderer::m_SceneData = new SceneData;

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewPort(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthoCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}
	void Renderer::EndScene()
	{

	}
	void Renderer::Submit(RendererSubmitData& data)
	{
		data.m_Shader->Bind();

		//Set uniforms here:


		data.m_VertexArray->Bind();
		RenderCommand::DrawIndexed(data.m_VertexArray);
	}
}