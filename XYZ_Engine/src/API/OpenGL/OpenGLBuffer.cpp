#include "stdafx.h"
#include "OpenGLBuffer.h"

#include <GL/glew.h>


namespace XYZ {
	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size, BufferUsage usage)
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

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
		: m_Size(size), m_Usage(BufferUsage::Dynamic)
	{
		glCreateBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
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

	void OpenGLVertexBuffer::Update(void* vertices, uint32_t size, uint32_t offset)
	{
		XYZ_ASSERT(m_Usage == BufferUsage::Dynamic, "Buffer does not have dynamic usage");
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, vertices);
	}

	void OpenGLVertexBuffer::Resize(float* vertices, uint32_t size)
	{
		m_Size = size;
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		switch (m_Usage)
		{
		case BufferUsage::Static:    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW); break;
		case BufferUsage::Dynamic:   glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW); break;
		}
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
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



	OpenGLShaderStorageBuffer::OpenGLShaderStorageBuffer(float* data, uint32_t size, BufferUsage usage)
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

	void OpenGLShaderStorageBuffer::BindBase(uint32_t index)
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBO);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, m_SSBO);
	}


	void OpenGLShaderStorageBuffer::BindRange(uint32_t offset, uint32_t size, uint32_t index)
	{		
		glBindBufferRange(GL_SHADER_STORAGE_BUFFER, index, m_SSBO, offset, size);
	}

	void OpenGLShaderStorageBuffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_SSBO);
	}

	void OpenGLShaderStorageBuffer::Update(const void* data, uint32_t size, uint32_t offset)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_SSBO);
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	}

	void OpenGLShaderStorageBuffer::Resize(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_SSBO);
		switch (m_Usage)
		{
		case BufferUsage::Static:    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW); break;
		case BufferUsage::Dynamic:   glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW); break;
		}
	}
	void OpenGLShaderStorageBuffer::GetSubData(void* buffer, uint32_t size, uint32_t offset)
	{
		XYZ_ASSERT(size + offset < m_Size, "Accesing data out of range");
		glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size, buffer);
	}
	OpenGLAtomicCounter::OpenGLAtomicCounter(uint32_t numOfCounters)
		: m_NumberOfCounters(numOfCounters),m_Counters(new uint32_t[numOfCounters])
	{
		// Make sure it is initialized as zero
		for (size_t i = 0; i < numOfCounters; ++i)
			m_Counters[i] = 0;

		glGenBuffers(1, &m_AC);
		glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, m_AC);
		glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(uint32_t) * numOfCounters, m_Counters, GL_DYNAMIC_DRAW);
	}
	OpenGLAtomicCounter::~OpenGLAtomicCounter()
	{
		glDeleteBuffers(1, &m_AC);
		delete[]m_Counters;
	}
	void OpenGLAtomicCounter::Reset()
	{
		for (size_t i = 0; i < m_NumberOfCounters; ++i)
			m_Counters[i] = 0;
		glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, m_AC);
		glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(uint32_t) * m_NumberOfCounters, m_Counters);
	}
	void OpenGLAtomicCounter::BindBase(uint32_t index)
	{
		glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, m_AC);
		glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER,index, m_AC);
	}
	void OpenGLAtomicCounter::Update(uint32_t* data, uint32_t count, uint32_t offset)
	{
		glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, m_AC);
		glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, offset * sizeof(uint32_t), sizeof(uint32_t) * m_NumberOfCounters, data);
	}
	uint32_t* OpenGLAtomicCounter::GetCounters()
	{
		glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, m_AC);
		glGetBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(uint32_t) * m_NumberOfCounters, m_Counters);		
		
		return m_Counters;
	}
	OpenGLIndirectBuffer::OpenGLIndirectBuffer(void* drawCommand, uint32_t size)
		:
		m_Size(size)
	{
		glGenBuffers(1, &m_IB);
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_IB);
		glBufferData(GL_DRAW_INDIRECT_BUFFER, size, drawCommand, GL_STATIC_DRAW);
	}
	OpenGLIndirectBuffer::~OpenGLIndirectBuffer()
	{
		glDeleteBuffers(1, &m_IB);
	}
	void OpenGLIndirectBuffer::Bind()
	{
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_IB);
	}
	void OpenGLIndirectBuffer::BindBase(uint32_t index)
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_IB);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, m_IB);
	}
}
