#include "stdafx.h"
#include "RendererSystem2D.h"
#include "RenderCommand.h"

namespace XYZ {
	RendererSystem2D::RendererSystem2D()
	{
		m_Signature.set(ECSManager::Get()->GetComponentType<Renderable2D>());
	}
	void RendererSystem2D::Add(Entity entity)
	{
		XYZ_LOG_INFO("Entity with id ", entity, " added");
		Component component;
		component.entity = entity;
		component.renderComponent = &ECSManager::Get()->GetComponent<Renderable2D>(entity);
		m_Components.push_back(component);

		for (size_t i = 0; i < m_Batches.size(); ++i)
		{
			if (component.renderComponent->material->GetSortKey() == m_Batches[i].material->GetSortKey()
				&& m_Batches[i].indexCount != c_MaxIndices)
			{
				return;
			}
		}
		RenderBatch batch;
		batch.material = component.renderComponent->material;
		batch.vao = VertexArray::Create();
		
		std::shared_ptr<VertexBuffer> VBO = VertexBuffer::Create(c_MaxVertices * sizeof(Vertex2D));
		VBO->SetLayout(XYZ::BufferLayout{
			{0, XYZ::ShaderDataType::Float4, "a_Color" },
			{1, XYZ::ShaderDataType::Float3, "a_Position" },
			{2, XYZ::ShaderDataType::Float2, "a_TexCoord" },
			{3, XYZ::ShaderDataType::Float,  "a_TextureID" },
		});
		batch.vao->AddVertexBuffer(VBO);

		batch.material = component.renderComponent->material;
		batch.bufferBase = new Vertex2D[c_MaxVertices];

		uint32_t* indices = new uint32_t[c_MaxIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < c_MaxIndices; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;
			
			offset += 4;
		}
		std::shared_ptr<IndexBuffer> IBO = IndexBuffer::Create(indices, c_MaxIndices);
		batch.vao->SetIndexBuffer(IBO);
		delete[] indices;

		
		m_Batches.push_back(batch);
	}
	void RendererSystem2D::Remove(Entity entity)
	{
	}
	void RendererSystem2D::BeginScene(const OrthoCamera& camera)
	{
		for (size_t i = 0; i < m_Batches.size(); ++i)
		{
			m_Batches[i].material->Set("u_ViewProjection", camera.GetViewProjectionMatrix());
			m_Batches[i].bufferPtr = m_Batches[i].bufferBase;
			m_Batches[i].indexCount = 0;
		}

		// TODO TEMPORARY
		for (auto it : m_Components)
		{
			for (size_t i = 0; i < m_Batches.size(); ++i)
			{
				if (it.renderComponent->material->GetSortKey() == m_Batches[i].material->GetSortKey()
					&& m_Batches[i].indexCount != c_MaxIndices)
				{
					m_Batches[i].Submit(*it.renderComponent);
					break;
				}
			}
		}
	}
	void RendererSystem2D::EndScene()
	{
		Flush();
	}
	void RendererSystem2D::Flush()
	{
		for (size_t i = 0; i < m_Batches.size(); ++i)
		{
			uint32_t dataSize = (uint8_t*)m_Batches[i].bufferPtr - (uint8_t*)m_Batches[i].bufferBase;
			m_Batches[i].vao->GetVertexBuffer()[0]->Update(m_Batches[i].bufferBase, dataSize);
			
			m_Batches[i].material->Bind();
			m_Batches[i].vao->Bind();
			RenderCommand::DrawIndexed(m_Batches[i].vao, m_Batches[i].indexCount);
		}
	}
	bool RendererSystem2D::Contains(Entity entity)
	{
		std::sort(m_Components.begin(), m_Components.end(), Component());
		int position = binarySearch(0, (int)m_Components.size() - 1, entity, m_Components);
		if (position == -1 || m_Components.empty())
			return false;

		return true;
	}
	void RendererSystem2D::RenderBatch::Submit(const Renderable2D& renderable)
	{
		if (renderable.visible)
		{
			bufferPtr->position = { renderable.position.x - renderable.size.x/2.0f,renderable.position.y - renderable.size.y/2.0f,renderable.position.z };
			bufferPtr->color = renderable.color;
			bufferPtr->texCoord.x = renderable.texCoord.x;
			bufferPtr->texCoord.y = renderable.texCoord.y;
			bufferPtr->textureID = renderable.textureID;
			bufferPtr++;

			bufferPtr->position = { renderable.position.x + renderable.size.x/2.0f,renderable.position.y - renderable.size.y / 2.0f,renderable.position.z };
			bufferPtr->color = renderable.color;
			bufferPtr->texCoord.x = renderable.texCoord.z;
			bufferPtr->texCoord.y = renderable.texCoord.y;
			bufferPtr->textureID = renderable.textureID;
			bufferPtr++;

			bufferPtr->position = { renderable.position.x + renderable.size.x/2.0f,renderable.position.y + renderable.size.y/2.0f,renderable.position.z };
			bufferPtr->color = renderable.color;
			bufferPtr->texCoord.x = renderable.texCoord.z;
			bufferPtr->texCoord.y = renderable.texCoord.w;
			bufferPtr->textureID = renderable.textureID;
			bufferPtr++;

			bufferPtr->position = { renderable.position.x - renderable.size.x/2.0f,renderable.position.y + renderable.size.y/2.0f,renderable.position.z };
			bufferPtr->color = renderable.color;
			bufferPtr->texCoord.x = renderable.texCoord.x;
			bufferPtr->texCoord.y = renderable.texCoord.w;
			bufferPtr->textureID = renderable.textureID;
			bufferPtr++;

			indexCount += 6;
		}
	}
}