#include <zr_pch.h>

#include <glad/glad.h>
#include "GL_ERR_CHECK.h"

#include "OpenGLRendererAPI.h"
#include "../../Core/Profiller.h"

namespace zr
{
	unsigned char OpenGLRendererAPI::sState = 0X0;

	OpenGLRendererAPI::OpenGLRendererAPI() :
		RendererAPI()
	{
	}

	OpenGLRendererAPI::~OpenGLRendererAPI()
	{
	}

	void OpenGLRendererAPI::init()
	{
		ZR_PROFILER_FUNCTION();

		setBlendState(true);
		setDepthTestState(true);
	}

	void OpenGLRendererAPI::drawIndexed(const Ref<VertexArray>& vertexArray)
	{
		GL_ERR_CHECK(glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr));
	}

	void OpenGLRendererAPI::drawArrays(RendererAPI::DrawPrimitive primitive, unsigned offset, unsigned count)
	{
		GL_ERR_CHECK(glDrawArrays(PrimitiveTypeToOpenGLPrimitiveType(primitive), offset, count));
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
		return OpenGLRendererAPI::sState & StateBits::DepthBit;
	}

	void OpenGLRendererAPI::setDepthTestState(bool depthEnabled)
	{
		if (depthEnabled && !(OpenGLRendererAPI::sState & StateBits::DepthBit)) {
			GL_ERR_CHECK(glEnable(GL_DEPTH_TEST));
			glDepthFunc(GL_LEQUAL);
			OpenGLRendererAPI::sState |= StateBits::DepthBit;
		}
		else {
			if (OpenGLRendererAPI::sState & StateBits::DepthBit) {
				GL_ERR_CHECK(glDisable(GL_DEPTH_TEST));
				OpenGLRendererAPI::sState &= ~StateBits::DepthBit;
			}
		}
	}

	void OpenGLRendererAPI::setBlendState(bool blendEnabled)
	{
		if (blendEnabled && !(OpenGLRendererAPI::sState & StateBits::BlendBit)) {
			GL_ERR_CHECK(glEnable(GL_BLEND));
			GL_ERR_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
			OpenGLRendererAPI::sState |= StateBits::BlendBit;
		}
		else {
			if (OpenGLRendererAPI::sState & StateBits::BlendBit) {
				GL_ERR_CHECK(glDisable(GL_BLEND));
				OpenGLRendererAPI::sState &= ~StateBits::BlendBit;
			}
		}
	}

	bool OpenGLRendererAPI::getBlendState()
	{
		return OpenGLRendererAPI::sState & StateBits::BlendBit;
	}
}
