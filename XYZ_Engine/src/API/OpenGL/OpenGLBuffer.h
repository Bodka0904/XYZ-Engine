#pragma once
#include "XYZ/Renderer/Buffer.h"


namespace XYZ {
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, unsigned int size, BufferUsage usage);
		~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void UnBind() const override;
		virtual void Update(float* vertices, unsigned int size, int offset = 0) override;
		virtual void Resize(float* vertices, unsigned int size) override;

		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; };

		virtual const BufferLayout& GetLayout() const override { return m_Layout; };
	private:
		BufferUsage m_Usage;
		unsigned int m_Size;
		unsigned int m_VBO;
		BufferLayout m_Layout;

	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, unsigned int count);
		~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual unsigned int GetCount() const override { return m_Count; }
	private:
		unsigned int m_IBO;
		unsigned int m_Count;
	};

	class OpenGLShaderStorageBuffer : public ShaderStorageBuffer
	{
	public:
		OpenGLShaderStorageBuffer(float* data, unsigned int size, BufferUsage usage);
		~OpenGLShaderStorageBuffer();

		virtual void BindRange(int offset, int size, unsigned int index) override;
		virtual void Bind() override;
		virtual void Update(const void* data, unsigned int size, int offset = 0) override;
		virtual void Resize(const void* data, unsigned int size) override;

		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; };;
		virtual const BufferLayout& GetLayout() const override { return m_Layout; };

	private:
		BufferUsage m_Usage;
		unsigned int m_Size;
		unsigned int m_SSBO;
		BufferLayout m_Layout;
	};

}