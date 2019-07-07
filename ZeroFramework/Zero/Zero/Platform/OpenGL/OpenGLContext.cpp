#include <zr_pch.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "OpenGLContext.h"

namespace zr
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) :
		mWindowHandle(windowHandle)
	{
		if (!windowHandle) {
			throw std::runtime_error("Handle is null!");
		}
	}

	OpenGLContext::~OpenGLContext()
	{

	}

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
