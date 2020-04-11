#include "stdafx.h"
#include "RendererBatchSystem2D.h"
#include "RenderCommandQueue.h"
#include "Renderer2D.h"
#include "RenderCommand.h"

namespace XYZ {
	struct Vertex2D
	{
		glm::vec4 color;
		glm::vec3 position;
		glm::vec2 texCoord;
		float	  textureID;
	};

	struct Renderer2DData
	{
		void Submit(const Renderable2D& renderable);
		void Reset();

		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;

		std::shared_ptr<Material> Material;
		std::shared_ptr<VertexArray> QuadVertexArray;
		std::shared_ptr<VertexBuffer> QuadVertexBuffer;

		uint32_t IndexCount = 0;
		Vertex2D* BufferBase = nullptr;
		Vertex2D* BufferPtr = nullptr;

	};
	
	static void Render(std::shared_ptr<Material> material, std::shared_ptr<VertexArray> vao, uint32_t count)
	{
		material->Bind();
		vao->Bind();
		RenderCommand::DrawIndexed(vao, count);
	}

	static void SubmitCommand(Renderer2DData& data)
	{
		uint32_t dataSize = (uint8_t*)data.BufferPtr - (uint8_t*)data.BufferBase;
		if (dataSize != 0)
		{
			data.QuadVertexBuffer->Update(data.BufferBase, dataSize);
			Command<std::shared_ptr<Material>, std::shared_ptr<VertexArray>, uint32_t> cmd(Render, data.Material, data.QuadVertexArray, data.IndexCount);
			Renderer2D::Submit(cmd, sizeof(cmd));
			data.Reset();
		}
	}

	static Renderer2DData s_Data;

	RendererBatchSystem2D::RendererBatchSystem2D()
	{
		m_Signature.set(ECSManager::Get()->GetComponentType<Renderable2D>());
		s_Data.Reset();
	}


	void RendererBatchSystem2D::Add(Entity entity)
	{
		auto renderable = &ECSManager::Get()->GetComponent<Renderable2D>(entity);
		auto key = renderable->material->GetSortKey();

		if (renderable->visible)
		{
			XYZ_LOG_INFO("Entity with id ", entity, " added");
			m_Entities.push_back(entity);

			if (renderable->material->GetSortKey() & RenderFlags::TransparentFlag)
				m_TransparentGroup.AddRenderable(renderable);			
			else
				m_OpaqueGroup.AddRenderable(renderable);
			
		}
	}

	void RendererBatchSystem2D::Remove(Entity entity)
	{
		auto it = std::find(m_Entities.begin(), m_Entities.end(), entity);
		if (it != m_Entities.end())
		{
			auto renderable = &ECSManager::Get()->GetComponent<Renderable2D>(entity);
			if (renderable->material->GetSortKey() & RenderFlags::TransparentFlag)
				m_TransparentGroup.RemoveRenderable(renderable);
			else
				m_OpaqueGroup.RemoveRenderable(renderable);


			XYZ_LOG_INFO("Entity with id ", entity, " removed");
			m_Entities.erase(it);
		}
	}
	bool RendererBatchSystem2D::Contains(Entity entity)
	{
		auto it = std::find(m_Entities.begin(), m_Entities.end(), entity);
		if (it != m_Entities.end())
			return true;

		return false;
	}

	void RendererBatchSystem2D::SubmitToRenderer()
	{
		for (auto it : m_OpaqueGroup.GetRenderables())
		{
			s_Data.Material = it.first;
			for (auto renderable : it.second)
			{
				if (s_Data.IndexCount < s_Data.MaxIndices)
					s_Data.Submit(*renderable);
				else
					SubmitCommand(s_Data);
			}
			SubmitCommand(s_Data);
		}

		for (auto it : m_TransparentGroup.GetRenderables())
		{
			s_Data.Material = it.first;
			for (auto renderable : it.second)
			{
				if (s_Data.IndexCount < s_Data.MaxIndices)
					s_Data.Submit(*renderable);
				else
					SubmitCommand(s_Data);
			}
			SubmitCommand(s_Data);
		}
	}

	void Renderer2DData::Submit(const Renderable2D& renderable)
	{
		BufferPtr->position = { renderable.position.x - renderable.size.x / 2.0f,renderable.position.y - renderable.size.y / 2.0f,renderable.position.z };
		BufferPtr->color = renderable.color;
		BufferPtr->texCoord.x = renderable.texCoord.x;
		BufferPtr->texCoord.y = renderable.texCoord.y;
		BufferPtr->textureID = renderable.textureID;
		BufferPtr++;

		BufferPtr->position = { renderable.position.x + renderable.size.x / 2.0f,renderable.position.y - renderable.size.y / 2.0f,renderable.position.z };
		BufferPtr->color = renderable.color;
		BufferPtr->texCoord.x = renderable.texCoord.z;
		BufferPtr->texCoord.y = renderable.texCoord.y;
		BufferPtr->textureID = renderable.textureID;
		BufferPtr++;

		BufferPtr->position = { renderable.position.x + renderable.size.x / 2.0f,renderable.position.y + renderable.size.y / 2.0f,renderable.position.z };
		BufferPtr->color = renderable.color;
		BufferPtr->texCoord.x = renderable.texCoord.z;
		BufferPtr->texCoord.y = renderable.texCoord.w;
		BufferPtr->textureID = renderable.textureID;
		BufferPtr++;

		BufferPtr->position = { renderable.position.x - renderable.size.x / 2.0f,renderable.position.y + renderable.size.y / 2.0f,renderable.position.z };
		BufferPtr->color = renderable.color;
		BufferPtr->texCoord.x = renderable.texCoord.x;
		BufferPtr->texCoord.y = renderable.texCoord.w;
		BufferPtr->textureID = renderable.textureID;
		BufferPtr++;

		IndexCount += 6;
	}

	void Renderer2DData::Reset()
	{
		if (!BufferBase)
		{
			BufferBase = new Vertex2D[MaxVertices];
			QuadVertexArray = VertexArray::Create();
			QuadVertexBuffer = VertexBuffer::Create(MaxVertices * sizeof(Vertex2D));
			QuadVertexBuffer->SetLayout(BufferLayout{
			{0, XYZ::ShaderDataType::Float4, "a_Color" },
			{1, XYZ::ShaderDataType::Float3, "a_Position" },
			{2, XYZ::ShaderDataType::Float2, "a_TexCoord" },
			{3, XYZ::ShaderDataType::Float,  "a_TextureID" },
				});
			QuadVertexArray->AddVertexBuffer(QuadVertexBuffer);


			uint32_t* quadIndices = new uint32_t[MaxIndices];
			uint32_t offset = 0;
			for (uint32_t i = 0; i < MaxIndices; i += 6)
			{
				quadIndices[i + 0] = offset + 0;
				quadIndices[i + 1] = offset + 1;
				quadIndices[i + 2] = offset + 2;

				quadIndices[i + 3] = offset + 2;
				quadIndices[i + 4] = offset + 3;
				quadIndices[i + 5] = offset + 0;

				offset += 4;
			}
			std::shared_ptr<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, MaxIndices);
			QuadVertexArray->SetIndexBuffer(quadIB);
			delete[] quadIndices;
		}
		BufferPtr = BufferBase;
		IndexCount = 0;
	}

}