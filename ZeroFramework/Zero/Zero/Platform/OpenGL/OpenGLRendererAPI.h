#include <Zero/Renderer/RendererAPI.h>

namespace zr
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		OpenGLRendererAPI();
		virtual ~OpenGLRendererAPI();

		// Inherited via RendererAPI
		virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
		virtual void setClearColor(float r, float g, float b, float a) override;
		virtual void clear() override;
		virtual void setViewportSize(unsigned width, unsigned height) override;
	};
}
