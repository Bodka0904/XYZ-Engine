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
	static Renderer2DData s_Data;


	static void SubmitCommand()
	{
		uint32_t dataSize = (uint8_t*)s_Data.BufferPtr - (uint8_t*)s_Data.BufferBase;
		if (dataSize != 0)
		{
			s_Data.QuadVertexBuffer->Update(s_Data.BufferBase, dataSize);
			s_Data.Material->Bind();
			s_Data.QuadVertexArray->Bind();
			RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.IndexCount);
			s_Data.Reset();
		}
	}

	RendererBatchSystem2D::RendererBatchSystem2D()
	{
		m_Signature.set(ECSManager::Get()->GetComponentType<Renderable2D>());
		m_Storage = ECSManager::Get()->GetComponentStorage<Renderable2D>();
		s_Data.Reset();
	}


	void RendererBatchSystem2D::Add(Entity entity)
	{
		auto renderable = ECSManager::Get()->GetComponent<Renderable2D>(entity);
		auto index = ECSManager::Get()->GetComponentIndex<Renderable2D>(entity);
		auto key = renderable->material->GetSortKey();

		if (renderable->visible & !(key & RenderFlags::InstancedFlag))
		{
			XYZ_LOG_INFO("Entity with id ", entity, " added");
			Component component;
			component.entity = entity;
			component.key = renderable->material->GetSortKey();
			m_Components.push_back(component);

			if (renderable->material->GetSortKey() & RenderFlags::TransparentFlag)
				m_TransparentGroup.Add(renderable->material, index);
			else
				m_OpaqueGroup.Add(renderable->material, index);
		}
	}

	void RendererBatchSystem2D::Remove(Entity entity)
	{
		auto it = std::find(m_Components.begin(), m_Components.end(), entity);
		if (it != m_Components.end())
		{
			auto renderable = ECSManager::Get()->GetComponent<Renderable2D>(entity);
			auto index = ECSManager::Get()->GetComponentIndex<Renderable2D>(entity);
			if (renderable->material->GetSortKey() & RenderFlags::TransparentFlag)
				m_TransparentGroup.Remove(renderable->material,index);
			else
				m_OpaqueGroup.Remove(renderable->material, index);

			XYZ_LOG_INFO("Entity with id ", entity, " removed");
			*it = std::move(m_Components.back());
			m_Components.pop_back();
		}
	}
	bool RendererBatchSystem2D::Contains(Entity entity)
	{
		auto it = std::find(m_Components.begin(), m_Components.end(), entity);
		if (it != m_Components.end())
			return true;

		return false;
	}

	// If renderable is updated and keys do not match, reinsert it and update key
	void RendererBatchSystem2D::EntityUpdated(Entity entity)
	{
		auto it = std::find(m_Components.begin(), m_Components.end(), entity);
		if (it != m_Components.end())
		{
			auto renderable = ECSManager::Get()->GetComponent<Renderable2D>(entity);
			auto index = ECSManager::Get()->GetComponentIndex<Renderable2D>(entity);
			if ((*it).key != renderable->material->GetSortKey())
			{
				if ((*it).key & RenderFlags::TransparentFlag)
					m_TransparentGroup.Remove(renderable->material,index);
				else
					m_OpaqueGroup.Remove(renderable->material,index);

				(*it).key = renderable->material->GetSortKey();
				Add(entity);
			}
		}
	}

	void RendererBatchSystem2D::SubmitToRenderer()
	{
		for (auto it : m_OpaqueGroup.GetGroup())
		{
			s_Data.Material = it.first;
			for (auto index : it.second)
			{	
				if (s_Data.IndexCount < s_Data.MaxIndices)
					s_Data.Submit((*m_Storage)[index]);
				else
					SubmitCommand();
			}
			SubmitCommand();
		}

		for (auto it : m_TransparentGroup.GetGroup())
		{
			s_Data.Material = it.first;
			for (auto index : it.second)
			{
				if (s_Data.IndexCount < s_Data.MaxIndices)
					s_Data.Submit((*m_Storage)[index]);
				else
					SubmitCommand();
			}
			SubmitCommand();
		}
	}

	void Renderer2DData::Submit(const Renderable2D& renderable)
	{
		BufferPtr->position = { renderable.position.x - renderable.size.x / 2.0f,renderable.position.y - renderable.size.y / 2.0f,renderable.position.z };
		BufferPtr->color = renderable.color;
		BufferPtr->texCoord.x = renderable.texCoord.x;
		BufferPtr->texCoord.y = renderable.texCoord.y;
		BufferPtr->textureID = (float)renderable.textureID;
		BufferPtr++;

		BufferPtr->position = { renderable.position.x + renderable.size.x / 2.0f,renderable.position.y - renderable.size.y / 2.0f,renderable.position.z };
		BufferPtr->color = renderable.color;
		BufferPtr->texCoord.x = renderable.texCoord.z;
		BufferPtr->texCoord.y = renderable.texCoord.y;
		BufferPtr->textureID = (float)renderable.textureID;
		BufferPtr++;

		BufferPtr->position = { renderable.position.x + renderable.size.x / 2.0f,renderable.position.y + renderable.size.y / 2.0f,renderable.position.z };
		BufferPtr->color = renderable.color;
		BufferPtr->texCoord.x = renderable.texCoord.z;
		BufferPtr->texCoord.y = renderable.texCoord.w;
		BufferPtr->textureID = (float)renderable.textureID;
		BufferPtr++;

		BufferPtr->position = { renderable.position.x - renderable.size.x / 2.0f,renderable.position.y + renderable.size.y / 2.0f,renderable.position.z };
		BufferPtr->color = renderable.color;
		BufferPtr->texCoord.x = renderable.texCoord.x;
		BufferPtr->texCoord.y = renderable.texCoord.w;
		BufferPtr->textureID = (float)renderable.textureID;
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