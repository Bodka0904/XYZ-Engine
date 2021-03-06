#include "stdafx.h"
#include "OpenGLRendererAPI.h"

#include <GL/glew.h>

namespace XYZ {
	void OpenGLRendererAPI::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		//glEnable(GL_DEPTH_TEST);
	}
	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount)
	{
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGLRendererAPI::DrawInstanced(const std::shared_ptr<VertexArray>& vertexArray, uint32_t count, uint32_t offset)
	{
		glDrawElementsInstancedBaseInstance(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0, count,offset);
	}
	void OpenGLRendererAPI::DrawInstancedIndirect(void* indirect)
	{
		glMemoryBarrier(GL_COMMAND_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);
		glDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, indirect);
	}
}