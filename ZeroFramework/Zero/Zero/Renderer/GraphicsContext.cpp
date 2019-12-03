#include <zr_pch.h>

#include <glad/glad.h>
#include "GraphicsContext.h"

#include "Zero/Core/Profiller.h"
#include "Zero/Core/Log.h"
#include "Zero/Platform/OpenGL/GL_ERR_CHECK.h"

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
		ZR_PROFILER_FUNCTION();

		glfwMakeContextCurrent(mWindowHandle);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			throw std::runtime_error("Failed to initialize the loader. (GLAD)");
		}

		const unsigned char* vendor = nullptr;
		const unsigned char* renderer = nullptr;
		const unsigned char* version = nullptr;
		GL_ERR_CHECK(vendor = glGetString(GL_VENDOR));
		GL_ERR_CHECK(renderer = glGetString(GL_RENDERER));
		GL_ERR_CHECK(version = glGetString(GL_VERSION));

		ZR_CORE_INFO("OpenGL Info: ");
		ZR_CORE_INFO("	Vendor: {0}", vendor);
		ZR_CORE_INFO("	Renderer: {0}", renderer);
		ZR_CORE_INFO("	Version: {0}", version);
	}

	void OpenGLContext::swapBuffers()
	{
		ZR_PROFILER_FUNCTION();

		glfwSwapBuffers(mWindowHandle);
	}
}
