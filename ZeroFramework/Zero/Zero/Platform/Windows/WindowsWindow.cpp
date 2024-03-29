#include <zr_pch.h>

#include <GLFW/glfw3.h>

#include "WindowsWindow.h"
#include "Zero/Core/Events.h"
#include "Zero/Platform/OpenGL/OpenGLContext.h"
#include "Zero/Core/Log.h"
#include "Zero/Core/Profiller.h"

#include "../../ImageReader.h"

namespace zr
{
	unsigned WindowsWindow::sGLFWWindowCount = 0;

	static void errorCallback(int error, const char* description)
	{
		std::cout << "GLFW ERROR: [" << error << "] " << description << "\n";
	}

	WindowsWindow::WindowsWindow(const WindowConfig& wc) :
		mWindowHandle(nullptr)
	{
		ZR_PROFILER_FUNCTION();
		mTitle = wc.Title;
		mData.Width = wc.Width;
		mData.Height = wc.Height;

		ZR_CORE_INFO("Creating window {0} ({1}, {2})", wc.Title, wc.Width, wc.Height);

		if (WindowsWindow::sGLFWWindowCount == 0) {
			ZR_CORE_INFO("Initializing GFLW");
			if (!glfwInit()) {
				throw std::runtime_error("Can't initialize an OpenGL context.");
			}
			glfwSetErrorCallback(errorCallback);
		}


		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_SAMPLES, 2);
		glfwWindowHint(GLFW_STENCIL_BITS, 24);
		glfwWindowHint(GLFW_RED_BITS, 8);
		glfwWindowHint(GLFW_GREEN_BITS, 8);
		glfwWindowHint(GLFW_BLUE_BITS, 8);

		mWindowHandle = glfwCreateWindow((int)wc.Width, (int)wc.Height, mTitle.c_str(), nullptr, nullptr);
		if (!mWindowHandle) {
			throw std::runtime_error("Window can't be initialized.");
		}

		++WindowsWindow::sGLFWWindowCount;
		// FIXME
		mContext = GraphicsContext::Create(mWindowHandle);
		mContext->init();

		setVSync(true);
		glfwSetWindowUserPointer(mWindowHandle, &mData);
		glfwSetInputMode(mWindowHandle, GLFW_STICKY_KEYS, GLFW_TRUE);

		glfwSetKeyCallback(mWindowHandle, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.callback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.callback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.callback(event);
					break;
				}
				default:
				break;
			}
		});

		glfwSetWindowSizeCallback(mWindowHandle, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.callback(event);
		});

		glfwSetMouseButtonCallback(mWindowHandle, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.callback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.callback(event);
					break;
				}
				default:
				break;
			}
		});

		glfwSetFramebufferSizeCallback(mWindowHandle, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.callback(event);
		});

		glfwSetWindowRefreshCallback(mWindowHandle, [](GLFWwindow* window) {
			glfwSwapBuffers(window);
		});

		glfwSetWindowCloseCallback(mWindowHandle, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.callback(event);
		});

		glfwSetScrollCallback(mWindowHandle, [](GLFWwindow* window, double xOffset, double yOffset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrollEvent event((float)xOffset, (float)yOffset);
			data.callback(event);
		});

		glfwSetCursorPosCallback(mWindowHandle, [](GLFWwindow* window, double xPos, double yPos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMoveEvent event((float)xPos, (float)yPos);
			data.callback(event);
		});

		glfwSetWindowFocusCallback(mWindowHandle, [](GLFWwindow* window, int focused) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			if (focused) {
				WindowGainedFocusEvent event;
				data.callback(event);
			}
			else {
				WindowLostFocusEvent event;
				data.callback(event);
			}
		});

		glfwSetCharCallback(mWindowHandle, [](GLFWwindow* window, unsigned int charCode) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(charCode);
			data.callback(event);
		});
	}

	Scope<Window> Window::Create(const WindowConfig& wc)
	{
		return CreateScope<WindowsWindow>(wc);
	}

	WindowsWindow::~WindowsWindow()
	{
		ZR_PROFILER_FUNCTION();

		glfwDestroyWindow(mWindowHandle);
		--WindowsWindow::sGLFWWindowCount;

		if (WindowsWindow::sGLFWWindowCount == 0) {
			ZR_CORE_INFO("Terminating GLFW");
			glfwTerminate();
		}
	}

	void WindowsWindow::onUpdate()
	{
		ZR_PROFILER_FUNCTION();

		glfwPollEvents();
		mContext->swapBuffers();
	}

	unsigned WindowsWindow::getWidth() const
	{
		return mData.Width;
	}

	unsigned WindowsWindow::getHeight() const
	{
		return mData.Height;
	}

	void* WindowsWindow::getNativeHandle() const
	{
		return mWindowHandle;
	}

	void WindowsWindow::setEventCallback(const EventCallback& callback)
	{
		mData.callback = callback;
	}

	void WindowsWindow::setVSync(bool enabled)
	{
		ZR_PROFILER_FUNCTION();

		if (enabled) {
			glfwSwapInterval(1);
		}
		else {
			glfwSwapInterval(0);
		}
		mData.VSync = enabled;
	}

	bool WindowsWindow::isVSync()
	{
		return mData.VSync;
	}

	void WindowsWindow::setIcon(const std::string& filePath) const
	{
		Image image;
		image.loadFromFile(filePath, false, true);
		setIcon(image);
	}

	void WindowsWindow::setIcon(const Image& image) const
	{
		GLFWimage images[1];
		images[0].width = image.getWidth();
		images[0].height = image.getHeight();
		images[0].pixels = (unsigned char*)image.getData();
		glfwSetWindowIcon(mWindowHandle, 1, images);
	}

	void WindowsWindow::captureMouseCursor(bool capture)
	{
		glfwSetInputMode(mWindowHandle, GLFW_CURSOR, (capture ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL));
	}
}
