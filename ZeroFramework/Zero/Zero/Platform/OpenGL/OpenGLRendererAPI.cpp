#include <zr_pch.h>

#include <glad/glad.h>
#include "GL_ERR_CHECK.h"

#include "OpenGLRendererAPI.h"

namespace zr
{
	OpenGLRendererAPI::OpenGLRendererAPI() :
		RendererAPI()
	{
	}

	OpenGLRendererAPI::~OpenGLRendererAPI()
	{
	}

	void OpenGLRendererAPI::drawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	{
		GL_ERR_CHECK(glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr));
	}

	void OpenGLRendererAPI::setClearColor(float r, float g, float b, float a)
	{
		GL_ERR_CHECK(glClearColor(r, g, b, a));
	}

	void OpenGLRendererAPI::clear()
	{
		GL_ERR_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void OpenGLRendererAPI::setViewportSize(unsigned width, unsigned height)
	{
		GL_ERR_CHECK(glViewport(0, 0, width, height));
	}
}
