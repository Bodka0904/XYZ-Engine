#include "stdafx.h"
#include "Renderer2D.h"

#include <memory>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture.h"
#include "Shader.h"
#include "Material.h"
#include "Renderer.h"
#include "RenderCommand.h"
#include "VertexArray.h"


namespace XYZ {
	struct Renderer2DStorage
	{
		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<Shader> m_TextureShader;
		std::shared_ptr<Texture2D> m_WhiteTexture;
	};

	static std::unique_ptr<Renderer2DStorage> s_Data = 0;
	static std::shared_ptr<Material> s_Material = 0;

	void Renderer2D::Init()
	{
		if (!s_Data) s_Data = std::make_unique<Renderer2DStorage>();
		s_Data->m_VertexArray = VertexArray::Create();

		
		float squareVert[] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};


		std::shared_ptr<VertexBuffer> squareVB;
		squareVB = VertexBuffer::Create(squareVert, sizeof(squareVert), BufferUsage::Static);
		squareVB->SetLayout({
			{  0, ShaderDataType::Float3, "a_Position" },
			{  1, ShaderDataType::Float2, "a_TexCoord" }
			});
		s_Data->m_VertexArray->AddVertexBuffer(squareVB);

		uint32_t squareInd[] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB = IndexBuffer::Create(squareInd, sizeof(squareInd) / sizeof(uint32_t));
		s_Data->m_VertexArray->SetIndexBuffer(squareIB);

		uint32_t whiteTextData = 0xffffffff;
		s_Data->m_WhiteTexture = Texture2D::Create(TextureFormat::RGB, TextureWrap::Clamp, 1, 1);
		s_Data->m_WhiteTexture->SetData(&whiteTextData, sizeof(whiteTextData));

		
		s_Data->m_TextureShader = Shader::Create("TextureShader", "../XYZ_Engine/Assets/Shaders/DefaultShader.glsl");
	}

	void Renderer2D::Shutdown()
	{
		s_Data.reset();
	}

	void Renderer2D::BeginScene(const OrthoCamera& camera)
	{
		s_Data->m_TextureShader->Bind();
		//s_Data->m_TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
	}

}