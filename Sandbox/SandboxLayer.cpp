#include <pch.h>

#include "SandboxLayer.h"

#include "Zero/Zero/Input.h"
#include "Zero/Zero/Events.h"
#include "Zero/Zero/InputCodes.h"

namespace lp
{
	SandboxLayer::SandboxLayer() :
		zr::Layer("SandboxLayer")
	{
	}

	SandboxLayer::~SandboxLayer()
	{
	}

	void SandboxLayer::onAttach()
	{
	}

	void SandboxLayer::onDetach()
	{
	}

	void SandboxLayer::onUpdate()
	{
		if (zr::Input::isKeyPressed(zr::Keyboard::Escape)) {
			std::cout << "ESCAPE Key is pressed.\n";
		}

		if (zr::Input::isKeyPressed(zr::Keyboard::Tab)) {
			std::cout << "TAB Key is pressed.\n";
		}

		if (zr::Input::isMouseButtonPressed(zr::MouseButton::Button0)) {
			std::cout << "LEFT MOUSE KEY is pressed.\n";
		}

		if (zr::Input::isMouseButtonPressed(zr::MouseButton::Button1)) {
			std::cout << "RIGHT MOUSE KEY is pressed.\n";
		}
	}

	void SandboxLayer::OnImGuiRender()
	{
	}

	void SandboxLayer::onEvent(zr::Event& e)
	{
		if (e.getType() == zr::EventType::KeyPressed) {
			if (dynamic_cast<zr::KeyPressedEvent&>(e).getKeyCode() == static_cast<int>(zr::Keyboard::Enter)) {
				std::cout << "ENTER key has just been pressed\n";
			}
		}
	}
}
