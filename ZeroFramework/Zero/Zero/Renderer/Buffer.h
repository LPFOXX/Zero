#pragma once

namespace zr
{
	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static unsigned ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type) {
			case ShaderDataType::Float:    return 4;
			case ShaderDataType::Float2:   return 4 * 2;
			case ShaderDataType::Float3:   return 4 * 3;
			case ShaderDataType::Float4:   return 4 * 4;
			case ShaderDataType::Mat3:     return 4 * 3 * 3;
			case ShaderDataType::Mat4:     return 4 * 4 * 4;
			case ShaderDataType::Int:      return 4;
			case ShaderDataType::Int2:     return 4 * 2;
			case ShaderDataType::Int3:     return 4 * 3;
			case ShaderDataType::Int4:     return 4 * 4;
			case ShaderDataType::Bool:     return 1;
		}

		return 0;
	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		unsigned Size;
		unsigned Offset;
		bool Normalized;

		BufferElement()
		{
		}

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{
		}

		unsigned getComponentCount() const
		{
			switch (Type) {
				case ShaderDataType::Float:   return 1;
				case ShaderDataType::Float2:  return 2;
				case ShaderDataType::Float3:  return 3;
				case ShaderDataType::Float4:  return 4;
				case ShaderDataType::Mat3:    return 3 * 3;
				case ShaderDataType::Mat4:    return 4 * 4;
				case ShaderDataType::Int:     return 1;
				case ShaderDataType::Int2:    return 2;
				case ShaderDataType::Int3:    return 3;
				case ShaderDataType::Int4:    return 4;
				case ShaderDataType::Bool:    return 1;
			}

			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout()
		{
		}

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: mElements(elements)
		{
			calculateOffsetsAndStride();
		}

		inline unsigned getStride() const
		{
			return mStride;
		}
		inline const std::vector<BufferElement>& getElements() const
		{
			return mElements;
		}

		std::vector<BufferElement>::iterator begin()
		{
			return mElements.begin();
		}
		std::vector<BufferElement>::iterator end()
		{
			return mElements.end();
		}
		std::vector<BufferElement>::const_iterator begin() const
		{
			return mElements.begin();
		}
		std::vector<BufferElement>::const_iterator end() const
		{
			return mElements.end();
		}
	private:
		void calculateOffsetsAndStride()
		{
			unsigned offset = 0;
			mStride = 0;
			for (auto& element : mElements) {
				element.Offset = offset;
				offset += element.Size;
				mStride += element.Size;
			}
		}
	private:
		std::vector<BufferElement> mElements;
		unsigned mStride = 0;
	};


	enum class DrawMode
	{
		Static,
		Dynamic
	};

	class VertexBuffer
	{
	public:
		VertexBuffer();
		virtual ~VertexBuffer();

		static VertexBuffer* Create(float* data, unsigned size, DrawMode drawMode);

		virtual void setData(float* data, unsigned size) = 0;
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual const BufferLayout& getLayout() const = 0;
		virtual void setLayout(const BufferLayout& layout) = 0;
	};

	class IndexBuffer
	{
	public:
		IndexBuffer();
		virtual ~IndexBuffer();

		static IndexBuffer* Create(unsigned* data, unsigned count, DrawMode drawMode);

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual unsigned getCount() const = 0;
	};

	class VertexArray
	{
	public:
		VertexArray();
		virtual ~VertexArray();

		static VertexArray* Create();

		virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
		virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;
		virtual void setLayout(const BufferLayout&) = 0;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const = 0;
		virtual const std::shared_ptr<IndexBuffer>& getIndexBuffer() const = 0;

		virtual void bind() = 0;
		virtual void unbind() = 0;
	};
}
