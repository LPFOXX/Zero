#pragma once

#include "../../Renderer/GraphicsContext.h"

namespace zr
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		virtual ~OpenGLContext();

		// Inherited via GraphicsContext
		virtual void init() override;
		virtual void swapBuffers() override;

	private:
		GLFWwindow* mWindowHandle;
	};
}
