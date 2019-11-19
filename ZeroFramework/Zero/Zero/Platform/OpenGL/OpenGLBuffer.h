#pragma once

#include <glad/glad.h>

#include "Zero/Renderer/Buffer.h"
#include "GL_ERR_CHECK.h"

namespace zr
{
	namespace OpenGL
	{
		GLenum getOpenGLDrawMode(DrawMode drawMode);
	}

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(void* data, unsigned size, DrawMode drawMode);
		virtual ~OpenGLVertexBuffer();

		// Inherited via VertexBuffer
		virtual void setData(float* data, unsigned size) override;
		virtual void bind() const override;
		virtual void unbind() const override;
		virtual const BufferLayout& getLayout() const override;
		virtual void setLayout(const BufferLayout& layout) override;
		virtual unsigned computeNextLayoutIndex() override;
		virtual unsigned getMaxLayoutIndex() override;

	private:
		unsigned mId;
		BufferLayout mLayout;
		unsigned mMaxLayoutIndex;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(unsigned* data, unsigned count, DrawMode drawMode);
		virtual ~OpenGLIndexBuffer();

		// Inherited via VertexBuffer
		virtual void bind() const override;
		virtual void unbind() const override;
		virtual unsigned getCount() const override;
		virtual void setData(unsigned* data, unsigned count) override;

	private:
		unsigned mId;
		unsigned mCount;
	};
}
