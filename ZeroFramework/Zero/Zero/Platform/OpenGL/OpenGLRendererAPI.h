
#include "../../vendor/glad/include/glad/glad.h"
#include <Zero/Renderer/RendererAPI.h>

namespace zr
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		OpenGLRendererAPI();
		virtual ~OpenGLRendererAPI();

		// Inherited via RendererAPI
		virtual void init() override;
		virtual void drawIndexed(const Ref<VertexArray>& vertexArray) override;
		virtual void drawArrays(RendererAPI::DrawPrimitive primitive, unsigned offset, unsigned count) override;
		virtual void setClearColor(float r, float g, float b, float a) override;
		virtual void clear(unsigned bufferBits) override;
		virtual void setViewportSize(unsigned width, unsigned height) override;
		virtual void setCullingFacesState(bool enabled, CullFace facesToCull) override;
		virtual bool getDepthTestState() override;
		virtual void setDepthTestState(bool depthEnabled) override;
		virtual void setBlendState(bool blendEnabled) override;
		virtual bool getBlendState() override;

	private:
		static unsigned PrimitiveTypeToOpenGLPrimitiveType(RendererAPI::DrawPrimitive drawPrimitive)
		{
			switch (drawPrimitive) {
				case RendererAPI::DrawPrimitive::TriangleFan: return GL_TRIANGLE_FAN;
				case zr::RendererAPI::DrawPrimitive::TriangleStrip: return GL_TRIANGLE_STRIP;
				default: return GL_TRIANGLES;
			}
			return GL_TRIANGLES;
		}

	private:
		static unsigned char sState;
	};
}
