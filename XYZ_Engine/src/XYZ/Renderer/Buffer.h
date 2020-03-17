#pragma once
#include <memory>

namespace XYZ {
	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};
	static unsigned int ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:		return 4;
		case ShaderDataType::Float2:	return 4 * 2;
		case ShaderDataType::Float3:	return 4 * 3;
		case ShaderDataType::Float4:	return 4 * 4;
		case ShaderDataType::Mat3:		return 4 * 3 * 3;
		case ShaderDataType::Mat4:		return 4 * 4 * 4;
		case ShaderDataType::Int:		return 4;
		case ShaderDataType::Int2:		return 4 * 2;
		case ShaderDataType::Int3:		return 4 * 3;
		case ShaderDataType::Int4:		return 4 * 4;
		case ShaderDataType::Bool:		return 1;
		}
		XYZ_ASSERT(false, "Buffer: Unknown ShaderDataType");
		return 0;
	}
	struct BufferElement
	{
		BufferElement(unsigned int index, ShaderDataType type, const std::string& name, unsigned int divisor = 0)
			: Index(index), Type(type), Divisor(divisor), Size(ShaderDataTypeSize(type)), Offset(0)
		{}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
			case ShaderDataType::Bool:   return 1;
			case ShaderDataType::Float:  return 1;
			case ShaderDataType::Float2: return 2;
			case ShaderDataType::Float3: return 3;
			case ShaderDataType::Float4: return 4;
			case ShaderDataType::Int:    return 1;
			case ShaderDataType::Int2:   return 2;
			case ShaderDataType::Int3:   return 3;
			case ShaderDataType::Int4:   return 4;
			case ShaderDataType::Mat3:   return 9;
			case ShaderDataType::Mat4:   return 16;
			}
			XYZ_ASSERT(false, "ShaderDataTypeSize(ShaderDataType::None)");
			return 0;
		}

		ShaderDataType Type;
		unsigned int   Size;
		unsigned int   Offset;
		unsigned int   Index;
		unsigned int   Divisor;
	};


	class BufferLayout
	{
	public:
		BufferLayout() {};
		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements)
		{
			CreateMat4();
			CalculateOffsetsAndStride();
		}

		inline const uint32_t& GetStride() const { return m_Stride; }
		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		auto begin() { return m_Elements.begin(); }
		auto end() { return m_Elements.end(); }
		auto begin() const { return m_Elements.begin(); }
		auto end() const { return m_Elements.end(); }
	private:
		inline void CalculateOffsetsAndStride()
		{
			unsigned int offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		};

		inline void CreateMat4()
		{
			for (auto& element : m_Elements)
			{
				if (element.Type == ShaderDataType::Mat4)
				{
					element.Type = ShaderDataType::Float4;
					element.Size = 4 * 4;

					BufferElement tmpElement = element;
					m_Elements.push_back(BufferElement(tmpElement.Index + 1, tmpElement.Type, "", tmpElement.Divisor));
					m_Elements.push_back(BufferElement(tmpElement.Index + 2, tmpElement.Type, "", tmpElement.Divisor));
					m_Elements.push_back(BufferElement(tmpElement.Index + 3, tmpElement.Type, "", tmpElement.Divisor));
				}
			}
		}

	private:
		std::vector<BufferElement> m_Elements;
		unsigned int m_Stride = 0;
	};

	enum class BufferUsage
	{
		None = 0, Static = 1, Dynamic = 2
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		virtual void Update(float* vertices, unsigned int size, int offset = 0) = 0;
		virtual void Resize(float* vertices, unsigned int size) = 0;

		virtual void SetLayout(const BufferLayout& layout) = 0;

		virtual const BufferLayout& GetLayout() const = 0;

		static std::shared_ptr<VertexBuffer> Create(float* vertices, unsigned int size, BufferUsage usage = BufferUsage::Static);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual unsigned int GetCount() const = 0;

		static std::shared_ptr<IndexBuffer> Create(uint32_t* indices, unsigned int count);
	};

	class ShaderStorageBuffer
	{
	public:
		virtual ~ShaderStorageBuffer() = default;

		virtual void BindRange(int offset, int size, unsigned int index) = 0;
		virtual void Bind() = 0;
		virtual void Update(const void* vertices, unsigned int size, int offset = 0) = 0;
		virtual void Resize(const void* vertices, unsigned int size) = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;

		static std::shared_ptr<ShaderStorageBuffer> Create(float* vertices, unsigned int size, BufferUsage usage = BufferUsage::Dynamic);
	};

}