#include <zr_pch.h>

#include "Zero/Application.h"
#include "WindowsInput.h"

namespace zr
{
	std::unique_ptr<Input> Input::sInstance = std::unique_ptr<Input>(new WindowsInput);

	WindowsInput::WindowsInput() :
		Input()
	{
	}

	WindowsInput::~WindowsInput()
	{
	}

	bool WindowsInput::isKeyPressedImpl(Keyboard key)
	{
		auto& window = Application::GetInstance().getWindow();
		auto state = glfwGetKey(static_cast<GLFWwindow*>(window.getNativeHandle()), static_cast<int>(key));
		return (state == GLFW_PRESS) || (state == GLFW_REPEAT);
	}

	bool WindowsInput::isMouseButtonPressedImpl(MouseButton button)
	{
		auto& window = Application::GetInstance().getWindow();
		return glfwGetMouseButton(static_cast<GLFWwindow*>(window.getNativeHandle()), static_cast<int>(button)) == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::getMousePositionImpl()
	{
		return std::make_pair(getMouseX(), getMouseY());
	}

	float WindowsInput::getMouseXImpl()
	{
		auto& window = Application::GetInstance().getWindow();
		double xPos = 0.0;
		glfwGetCursorPos(static_cast<GLFWwindow*>(window.getNativeHandle()), &xPos, nullptr);
		return static_cast<float>(xPos);
	}

	float WindowsInput::getMouseYImpl()
	{
		auto& window = Application::GetInstance().getWindow();
		double yPos = 0.0;
		glfwGetCursorPos(static_cast<GLFWwindow*>(window.getNativeHandle()), nullptr, &yPos);
		return static_cast<float>(yPos);
	}
}
