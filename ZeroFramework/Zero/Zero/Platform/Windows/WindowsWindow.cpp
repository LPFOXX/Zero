#include <zr_pch.h>

#include "WindowsWindow.h"
#include "../../Events.h"

namespace zr
{

	bool WindowsWindow::sGLFWInitialized = false;

	static void errorCallback(int error, const char* description)
	{
		std::cout << "GLFW ERROR: [" << error << "] " << description << "\n";
	}

	WindowsWindow::WindowsWindow(const WindowConfig& wc) :
		mWindowHandle(nullptr)
	{
		mTitle = wc.Title;
		mData.Width = wc.Width;
		mData.Height = wc.Height;

		if (!WindowsWindow::sGLFWInitialized) {
			if (!glfwInit()) {
				throw std::runtime_error("Can't initialize an OpenGL context.");
			}

			glfwSetErrorCallback(errorCallback);
			WindowsWindow::sGLFWInitialized = true;
		}

		mWindowHandle = glfwCreateWindow((int)wc.Width, (int)wc.Height, mTitle.c_str(), nullptr, nullptr);
		if (!mWindowHandle) {
			throw std::runtime_error("Window can't be initialized.");
		}

		glfwMakeContextCurrent(mWindowHandle);
		glfwSetWindowUserPointer(mWindowHandle, &mData);
		setVSync(true);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			throw std::runtime_error("Failed to initialize the loader. (GLAD)");
		}

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

	Window* Window::Create(const WindowConfig& wc)
	{
		return new WindowsWindow(wc);
	}

	WindowsWindow::~WindowsWindow()
	{
		glfwDestroyWindow(mWindowHandle);
		glfwTerminate();
	}

	void WindowsWindow::onUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(mWindowHandle);
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
		if (enabled) {
			glfwSwapInterval(1);
		}
		else {
			glfwSwapInterval(1);
		}
		mData.VSync = enabled;
	}

	bool WindowsWindow::isVSync()
	{
		return mData.VSync;
	}
}
