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
		virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
		virtual void setClearColor(float r, float g, float b, float a) override;
		virtual void clear(unsigned bufferBits) override;
		virtual void setViewportSize(unsigned width, unsigned height) override;
		virtual void setCullingFacesState(bool enabled, CullFace facesToCull) override;
		virtual bool getDepthTestState() override;
		virtual void setDepthTestState(bool state) override;
		virtual void enableBlend(bool blendEnabled) override;
		virtual bool isBlendEnabled() override;
	};
}
