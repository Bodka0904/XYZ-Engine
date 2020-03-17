#include "stdafx.h"
#include "OpenGLBuffer.h"

#include <GL/glew.h>


namespace XYZ {
	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, unsigned int size, BufferUsage usage)
		: m_Size(size), m_Usage(usage)
	{
		glCreateBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		switch (usage)
		{
		case BufferUsage::Static:    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW); break;
		case BufferUsage::Dynamic:   glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW); break;
		}
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_VBO);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	}

	void OpenGLVertexBuffer::UnBind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::Update(float* vertices, unsigned int size, int offset)
	{
		XYZ_ASSERT(m_Usage == BufferUsage::Dynamic, "Buffer does not have dynamic usage");
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, vertices);
	}

	void OpenGLVertexBuffer::Resize(float* vertices, unsigned int size)
	{
		m_Size = size;
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		switch (m_Usage)
		{
		case BufferUsage::Static:    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW); break;
		case BufferUsage::Dynamic:   glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW); break;
		}
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, unsigned int count)
		: m_Count(count)
	{
		glCreateBuffers(1, &m_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_IBO);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_IBO);
	}

	void OpenGLIndexBuffer::UnBind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}



	OpenGLShaderStorageBuffer::OpenGLShaderStorageBuffer(float* data, unsigned int size, BufferUsage usage)
		:m_Size(size), m_Usage(usage)
	{
		glGenBuffers(1, &m_SSBO);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBO);
		switch (m_Usage)
		{
		case BufferUsage::Static:    glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_STATIC_DRAW); break;
		case BufferUsage::Dynamic:   glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_DRAW); break;
		}
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	OpenGLShaderStorageBuffer::~OpenGLShaderStorageBuffer()
	{
		glDeleteBuffers(1, &m_SSBO);
	}

	void OpenGLShaderStorageBuffer::BindRange(int offset, int size, unsigned int index)
	{
		glBindBufferRange(GL_SHADER_STORAGE_BUFFER, index, m_SSBO, offset, size);
	}

	void OpenGLShaderStorageBuffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_SSBO);
	}

	void OpenGLShaderStorageBuffer::Update(const void* data, unsigned int size, int offset)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_SSBO);
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	}

	void OpenGLShaderStorageBuffer::Resize(const void* data, unsigned int size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_SSBO);
		switch (m_Usage)
		{
		case BufferUsage::Static:    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW); break;
		case BufferUsage::Dynamic:   glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW); break;
		}
	}
}

