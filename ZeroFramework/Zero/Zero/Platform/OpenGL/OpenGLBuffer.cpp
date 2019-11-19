#include <zr_pch.h>

#include "OpenGLBuffer.h"

namespace zr
{
	namespace OpenGL
	{
		GLenum getOpenGLDrawMode(DrawMode drawMode)
		{
			switch (drawMode) {
				case zr::DrawMode::Dynamic:
				{
					return GL_DYNAMIC_DRAW;
				}
				case zr::DrawMode::Streamed:
				{
					return GL_STREAM_DRAW;
				}
				case zr::DrawMode::Static:
				default:
				{
					return GL_STATIC_DRAW;
				}
				break;
			}
			return GL_STATIC_DRAW;
		}
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(void* data, unsigned size, DrawMode drawMode) :
		VertexBuffer(),
		mId(0),
		mMaxLayoutIndex(0U)
	{
		if (mId == 0) {
			GL_ERR_CHECK(glGenBuffers(1, &mId));
			GL_ERR_CHECK(glBindBuffer(GL_ARRAY_BUFFER, mId));
			GL_ERR_CHECK(glBufferData(GL_ARRAY_BUFFER, size, data, OpenGL::getOpenGLDrawMode(drawMode)));
		}
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		if (mId != 0U) {
			GL_ERR_CHECK(glDeleteBuffers(1, &mId));
			mId = 0U;
		}
	}

	void OpenGLVertexBuffer::setData(float* data, unsigned size)
	{
		// Bind the buffer and updates the data
		GL_ERR_CHECK(glBindBuffer(GL_ARRAY_BUFFER, mId));
		GL_ERR_CHECK(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
	}

	void OpenGLVertexBuffer::bind() const
	{
		GL_ERR_CHECK(glBindBuffer(GL_ARRAY_BUFFER, mId));
	}

	void OpenGLVertexBuffer::unbind() const
	{
		GL_ERR_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	const BufferLayout& OpenGLVertexBuffer::getLayout() const
	{
		return mLayout;
	}

	void OpenGLVertexBuffer::setLayout(const BufferLayout& layout)
	{
		mLayout = layout;
	}

	unsigned OpenGLVertexBuffer::computeNextLayoutIndex()
	{
		mMaxLayoutIndex = 0U;
		const auto& elements = mLayout.getElements();
		for (auto& element : elements) {
			if (element.VectorSize >= 2) {
				mMaxLayoutIndex += element.VectorSize;
			}
			else {
				++mMaxLayoutIndex;
			}
		}
		return mMaxLayoutIndex;
	}

	unsigned OpenGLVertexBuffer::getMaxLayoutIndex()
	{
		return mMaxLayoutIndex;
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned* data, unsigned count, DrawMode drawMode) :
		IndexBuffer(),
		mId(),
		mCount(count)
	{
		if (mId == 0U) {
			GL_ERR_CHECK(glGenBuffers(1, &mId));
			GL_ERR_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mId));
			GL_ERR_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned), data, OpenGL::getOpenGLDrawMode(drawMode)));
		}
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		if (mId != 0U) {
			GL_ERR_CHECK(glDeleteBuffers(1, &mId));
			mId = 0U;
		}
	}

	void OpenGLIndexBuffer::bind() const
	{
		GL_ERR_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mId));
	}

	void OpenGLIndexBuffer::unbind() const
	{
		GL_ERR_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	unsigned OpenGLIndexBuffer::getCount() const
	{
		return mCount;
	}

	void OpenGLIndexBuffer::setData(unsigned* data, unsigned count)
	{
		// Bind the buffer and updates the data
		GL_ERR_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mId));
		GL_ERR_CHECK(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(unsigned), data));
	}
}
