#pragma once

#include "../Core/Core.h"

namespace zr
{
	enum class ShaderDataType
	{
		None = 0,
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		Int,
		Int2,
		Int3,
		Int4,
		Bool
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

	static std::string ShaderDataTypeGLType(ShaderDataType type)
	{
		switch (type) {
			case ShaderDataType::Float:    return "float";
			case ShaderDataType::Float2:   return "vec2";
			case ShaderDataType::Float3:   return "vec3";
			case ShaderDataType::Float4:   return "vec4";
			case ShaderDataType::Mat3:     return "mat3";
			case ShaderDataType::Mat4:     return "mat4";
			case ShaderDataType::Int:      return "int";
			case ShaderDataType::Int2:     return "ivec2";
			case ShaderDataType::Int3:     return "ivec3";
			case ShaderDataType::Int4:     return "ivec4";
			case ShaderDataType::Bool:     return "bool";
		}

		return "";
	}

	static bool IsIntegerShaderDataType(ShaderDataType type)
	{
		return type == ShaderDataType::Int || type == ShaderDataType::Int2 || type == ShaderDataType::Int3 || type == ShaderDataType::Int4;
	}

	struct BufferElement
	{
		BufferElement() :
			Name(),
			Type(ShaderDataType::None),
			Size(0U),
			Offset(0U),
			Normalized(false),
			VectorSize(0),
			Divisor(0U)
		{
		}

		BufferElement(ShaderDataType type, const std::string& name, unsigned arraySize = 0U, bool normalized = false, unsigned divisor = 0U) :
			Name(name),
			Type(type),
			Size(ShaderDataTypeSize(type)),
			Offset(0),
			Normalized(normalized),
			VectorSize(arraySize),
			Divisor(divisor)
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

		std::string Name;
		ShaderDataType Type;
		unsigned Size;
		unsigned Offset;
		bool Normalized;
		unsigned VectorSize;	// When not 0 or 1 means the element is a vector
		unsigned Divisor;
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

		inline void addElement(BufferElement& element)
		{
			mElements.push_back(element);
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
				offset += element.Size * (element.VectorSize >= 2 ? element.VectorSize : 1);
				mStride += element.Size * (element.VectorSize >= 2 ? element.VectorSize : 1);
			}
		}
	private:
		std::vector<BufferElement> mElements;
		unsigned mStride = 0;
	};

	enum class DrawMode
	{
		Static,
		Dynamic,
		Streamed
	};

	class VertexBuffer
	{
	public:
		VertexBuffer();
		virtual ~VertexBuffer();

		static Ref<VertexBuffer> Create(void* data, unsigned size, DrawMode drawMode);

		virtual void setData(float* data, unsigned size) = 0;
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual const BufferLayout& getLayout() const = 0;
		virtual void setLayout(const BufferLayout& layout) = 0;
		virtual unsigned computeNextLayoutIndex() = 0;
		virtual unsigned getMaxLayoutIndex() = 0;
	};

	class IndexBuffer
	{
	public:
		struct Bounds
		{
			/**
			* offset in bytes
			*/
			void addBounds(unsigned offset, unsigned count)
			{
				Offset.push_back((void*)offset);
				Count.push_back(count);
			}

			inline int size() const
			{
				return Offset.size();
			}

			void clear()
			{
				Offset.clear();
				Offset.clear();
			}

			std::vector<void*> Offset;
			std::vector<int> Count;
		};

	public:
		IndexBuffer();
		virtual ~IndexBuffer();

		static Ref<IndexBuffer> Create(unsigned* data, unsigned count, DrawMode drawMode);

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual unsigned getCount() const = 0;
		virtual void setData(unsigned* data, unsigned count) = 0;
	};

	class VertexArray
	{
	public:
		VertexArray();
		virtual ~VertexArray();

		static Ref<VertexArray> Create(bool createAndBind = false);

		virtual void addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;
		virtual void setLayout(const BufferLayout&) = 0;

		virtual const std::vector<Ref<VertexBuffer>>& getVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& getIndexBuffer() const = 0;
		virtual std::string getShaderLayouts() const = 0;

		virtual void bind() = 0;
		virtual void unbind() = 0;
	};
}
