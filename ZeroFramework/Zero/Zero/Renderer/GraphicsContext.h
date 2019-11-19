#pragma once

#include "GLFW/glfw3.h"
#include "../Core/Core.h"
#include "Renderer.h"

namespace zr
{
	class GraphicsContext
	{
	public:
		GraphicsContext();
		virtual ~GraphicsContext();

		virtual void init() = 0;
		virtual void swapBuffers() = 0;

		static Scope<GraphicsContext> Create(void* windowHandle);
	};

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
