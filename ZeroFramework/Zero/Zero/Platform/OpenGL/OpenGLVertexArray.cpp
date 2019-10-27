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

	OpenGLVertexArray::OpenGLVertexArray(bool createAndBind) :
		VertexArray(),
		mId(0)
	{
		GL_ERR_CHECK(glGenVertexArrays(1, &mId));
		if (createAndBind) {
			GL_ERR_CHECK(glBindVertexArray(mId));
		}
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		if (mId != 0U) {
			GL_ERR_CHECK(glDeleteVertexArrays(1, &mId));
			mId = 0U;
		}
	}

	void OpenGLVertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		GL_ERR_CHECK(glBindVertexArray(mId));
		vertexBuffer->bind();

		unsigned index = computeNextIndexLocation();
		const auto& layout = vertexBuffer->getLayout();
		for (const auto& element : layout) {
			GL_ERR_CHECK(glEnableVertexAttribArray(index));
			if (
				element.Type == ShaderDataType::Int ||
				element.Type == ShaderDataType::Int2 ||
				element.Type == ShaderDataType::Int3 ||
				element.Type == ShaderDataType::Int4
				) {
				GL_ERR_CHECK(glVertexAttribIPointer(
					index,
					element.getComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					layout.getStride(),
					(const void*) element.Offset));
			}
			else {
				GL_ERR_CHECK(glVertexAttribPointer(
					index,											// index
					element.getComponentCount(),					// number of components (1|2|3|4)
					ShaderDataTypeToOpenGLBaseType(element.Type),	// type of each component
					element.Normalized ? GL_TRUE : GL_FALSE,		// whether or not to normalized fixed point values
					layout.getStride(),								// stride in bytes. if 0 then is thighly packed
					(const void*)element.Offset));					// offset from very first byte on the buffer
			}
			if (element.Divisor != 0U) {
				GL_ERR_CHECK(glVertexAttribDivisor(index, element.Divisor));
			}

			if (element.VectorSize >= 2) {
				index += element.VectorSize;
			}
			else {
				index++;
			}
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

	std::string OpenGLVertexArray::getShaderLayouts() const
	{
		std::stringstream ss;
		unsigned location = 0U;
		for (auto& vertexBuffer : mVertexBuffers) {
			const auto& layout = vertexBuffer->getLayout().getElements();
			for (auto& layoutElement : layout) {
				std::string& layoutIdentifier = layoutElement.Name + (layoutElement.VectorSize >= 2 ? "[" + std::to_string(layoutElement.VectorSize) + "]" : "");
				ss << "layout (location = " << location << ") in " << ShaderDataTypeGLType(layoutElement.Type) << " " << layoutIdentifier << ";\n";
				if (layoutElement.VectorSize >= 2) {
					location += layoutElement.VectorSize;
				}
				else {
					++location;
				}
			}
		}
		return ss.str();
	}

	unsigned OpenGLVertexArray::computeNextIndexLocation() const
	{
		unsigned returnValue = 0U;
		for (auto& vertexBuffer : mVertexBuffers) {
			returnValue += vertexBuffer->computeNextLayoutIndex();
		}
		return returnValue;
	}
}
