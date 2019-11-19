#include <zr_pch.h>

#include <glad/glad.h>
#include "GraphicsContext.h"

namespace zr
{
	GraphicsContext::GraphicsContext()
	{
	}

	GraphicsContext::~GraphicsContext()
	{
	}

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
			{
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
			}
			case RendererAPI::API::Direct3D:
			{
				return nullptr;
			}
			case RendererAPI::API::Vulkan:
			{
				return nullptr;
			}
			default:
			{
				return nullptr;
			}
		}
		return nullptr;
	}

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) :
		GraphicsContext(),
		mWindowHandle(windowHandle)
	{
		if (!windowHandle) {
			throw std::runtime_error("Handle is null!");
		}
	}

	OpenGLContext::~OpenGLContext()
	{

	}

	// Inherited via GraphicsContext
	void OpenGLContext::init()
	{
		glfwMakeContextCurrent(mWindowHandle);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			throw std::runtime_error("Failed to initialize the loader. (GLAD)");
		}
	}

	void OpenGLContext::swapBuffers()
	{
		glfwSwapBuffers(mWindowHandle);
	}
}
