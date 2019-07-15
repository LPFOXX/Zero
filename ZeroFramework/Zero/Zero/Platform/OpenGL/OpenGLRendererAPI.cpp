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

	void OpenGLRendererAPI::clear(unsigned bufferBits)
	{
		unsigned bitField = 0U;
		bitField |= (bufferBits & ClearBuffers::Color) ? GL_COLOR_BUFFER_BIT : 0;
		bitField |= (bufferBits & ClearBuffers::Depth) ? GL_DEPTH_BUFFER_BIT : 0;
		bitField |= (bufferBits & ClearBuffers::Stencil) ? GL_STENCIL_BUFFER_BIT : 0;
		GL_ERR_CHECK(glClear(bitField));
	}

	void OpenGLRendererAPI::setViewportSize(unsigned width, unsigned height)
	{
		GL_ERR_CHECK(glViewport(0, 0, width, height));
	}

	void OpenGLRendererAPI::setCullingFacesState(bool enabled, CullFace facesToCull)
	{
		if (!enabled) {
			GL_ERR_CHECK(glDisable(GL_CULL_FACE));
			return;
		}

		GL_ERR_CHECK(glEnable(GL_CULL_FACE));
		if (facesToCull == CullFace::Back) {
			GL_ERR_CHECK(glCullFace(GL_BACK));
		}
		else if (facesToCull == CullFace::Front) {
			GL_ERR_CHECK(glCullFace(GL_FRONT));
		}
		//glFrontFace(GL_CW);
	}

	bool OpenGLRendererAPI::getDepthTestState()
	{
		unsigned char wasDepthEnabled;
		GL_ERR_CHECK(wasDepthEnabled = glIsEnabled(GL_DEPTH_TEST));
		return (wasDepthEnabled == GL_TRUE ? true : false);
	}

	void OpenGLRendererAPI::setDepthTestState(bool state)
	{
		if (state) {
			GL_ERR_CHECK(glEnable(GL_DEPTH_TEST));
			glDepthFunc(GL_LEQUAL);
		}
		else {
			GL_ERR_CHECK(glDisable(GL_DEPTH_TEST));
		}
	}
}
