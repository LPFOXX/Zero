#include <zr_pch.h>

#include <glad/glad.h>

#include "OpenGLVertexArray.h"
#include "GL_ERR_CHECK.h"


namespace zr
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type) {
			case ShaderDataType::Float:    return GL_FLOAT;
			case ShaderDataType::Float2:   return GL_FLOAT;
			case ShaderDataType::Float3:   return GL_FLOAT;
			case ShaderDataType::Float4:   return GL_FLOAT;
			case ShaderDataType::Mat3:     return GL_FLOAT;
			case ShaderDataType::Mat4:     return GL_FLOAT;
			case ShaderDataType::Int:      return GL_INT;
			case ShaderDataType::Int2:     return GL_INT;
			case ShaderDataType::Int3:     return GL_INT;
			case ShaderDataType::Int4:     return GL_INT;
			case ShaderDataType::Bool:     return GL_BOOL;
		}
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray() :
		VertexArray(),
		mId(0),
		mAttributesBound(0U)
	{
		GL_ERR_CHECK(glGenVertexArrays(1, &mId));
		/*if (createAndBind) {
			GL_ERR_CHECK(glBindVertexArray(mId));
		}*/
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		if (mId != 0U) {
			GL_ERR_CHECK(glDeleteVertexArrays(1, &mId));
			mId = 0U;
		}
		mAttributesBound = 0U;
	}

	void OpenGLVertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		GL_ERR_CHECK(glBindVertexArray(mId));
		vertexBuffer->bind();

		unsigned index = 0;
		const auto& layout = vertexBuffer->getLayout();
		for (const auto& element : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.getComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.getStride(),
				(const void*)element.Offset);
			index++;
		}

		mVertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		GL_ERR_CHECK(glBindVertexArray(mId));
		indexBuffer->bind();

		mIndexBuffer = indexBuffer;
	}

	void OpenGLVertexArray::setLayout(const BufferLayout&)
	{
	}

	void OpenGLVertexArray::bind()
	{
		GL_ERR_CHECK(glBindVertexArray(mId));
	}

	void OpenGLVertexArray::unbind()
	{
		GL_ERR_CHECK(glBindVertexArray(0));
	}

	const std::vector<std::shared_ptr<VertexBuffer>>& OpenGLVertexArray::getVertexBuffers() const
	{
		return mVertexBuffers;
	}

	const std::shared_ptr<IndexBuffer>& OpenGLVertexArray::getIndexBuffer() const
	{
		return mIndexBuffer;
	}
}
