//#include <zr_pch.h>
#pragma once

#include <rttr/type>
#include <rttr/registration>
#include <rttr/registration_friend.h>

#include <iostream>
#include <string>

#include "../../Core/Input.h"
#include "../../Core/Events/Event.h"
#include "../RTTR/TestSprite.h"

#include "../../Platform/RTTR/RttrMetadata.h"

namespace zr
{
	static void HelloWorld()
	{
		std::cout << "Hello World\n";
	}

	static void HelloWorld1(int arg0)
	{
		std::cout << "Hello World1 " << arg0 << "\n";
	}

	static const char* HelloWorld2(int arg0, float arg1)
	{
		std::cout << "Hello World2 " << arg0 << " " << arg1 << "\n";
		//return std::to_string((float)arg0 * arg1).c_str();
		return "Hello World2";
	}

	static void StringTest(const char* arg0)
	{
		std::cout << "String Test: \"" << arg0 << "\"\n";
	}

	static void DefaultValueTest(int arg0, short arg1, unsigned arg2, float arg3 = 5.f)
	{
		std::cout << "Default Value Test: " << arg0 << " " << arg1 << " " << arg2 << " " << arg3 << "\n";
	}
}

static int KeyboardToInt(zr::Keyboard keyCode, bool& ok)
{
	ok = true;
	return (int)keyCode;
}

static int IntToKeyboard(zr::Keyboard keyCode, bool& ok)
{
	ok = true;
	return (int)keyCode;
}

RTTR_REGISTRATION
{
	using reg = rttr::registration;

reg::class_<test::Sprite>("Sprite")
.constructor<>()
.method("setPosition", &test::Sprite::setPosition)
.method("getPosition", static_cast<std::pair<float, float>(test::Sprite::*)()const>(&test::Sprite::getPosition))
.method("setSize", &test::Sprite::setSize)
.method("getSize", static_cast<std::pair<float, float>(test::Sprite::*)()const>(&test::Sprite::getSize))
.method("move", &test::Sprite::move)
.method("draw", &test::Sprite::draw)
.property("PublicX", &test::Sprite::PublicX)
.property("PublicY", &test::Sprite::PublicY)
.property("mX", &test::Sprite::mX, reg::private_access)
.property("mY", &test::Sprite::mY, reg::private_access);

	reg::class_<zr::Input>("Input")(rttr::metadata(zr::RTTRMetadata::Bool::StaticClass, true))
		.method("IsKeyPressed", static_cast<bool(*)(unsigned)>(&zr::Input::isKeyPressed))
		.method("IsMouseButtonPressed", static_cast<bool(*)(unsigned)>(&zr::Input::isMouseButtonPressed))
		.method("GetMousePosition", &zr::Input::getMousePosition)
		.method("GetMouseX", &zr::Input::getMouseX)
		.method("GetMouseY", &zr::Input::getMouseY);

	reg::method("HelloWorld", &zr::HelloWorld);
	reg::method("HelloWorld1", &zr::HelloWorld1);
	reg::method("HelloWorld2", &zr::HelloWorld2);
	reg::method("StringTest", &zr::StringTest);
	reg::method("DefaultValueTest", &zr::DefaultValueTest)(
		rttr::default_arguments(5.f)
	);

	reg::enumeration<zr::Keyboard>("Keyboard")(
		rttr::value("Space", zr::Keyboard::Space),
		rttr::value("Apostrophe", zr::Keyboard::Apostrophe),
		rttr::value("Comma", zr::Keyboard::Comma),
		rttr::value("Hyphen", zr::Keyboard::Hyphen),
		rttr::value("Period", zr::Keyboard::Period),
		rttr::value("Slash", zr::Keyboard::Slash),
		rttr::value("Num0", zr::Keyboard::Num0),
		rttr::value("Num1", zr::Keyboard::Num1),
		rttr::value("Num2", zr::Keyboard::Num2),
		rttr::value("Num3", zr::Keyboard::Num3),
		rttr::value("Num4", zr::Keyboard::Num4),
		rttr::value("Num5", zr::Keyboard::Num5),
		rttr::value("Num6", zr::Keyboard::Num6),
		rttr::value("Num7", zr::Keyboard::Num7),
		rttr::value("Num8", zr::Keyboard::Num8),
		rttr::value("Num9", zr::Keyboard::Num9),
		rttr::value("Semincolon", zr::Keyboard::Semincolon),
		rttr::value("Equal", zr::Keyboard::Equal),
		rttr::value("A", zr::Keyboard::A),
		rttr::value("B", zr::Keyboard::B),
		rttr::value("C", zr::Keyboard::C),
		rttr::value("D", zr::Keyboard::D),
		rttr::value("E", zr::Keyboard::E),
		rttr::value("F", zr::Keyboard::F),
		rttr::value("G", zr::Keyboard::G),
		rttr::value("H", zr::Keyboard::H),
		rttr::value("I", zr::Keyboard::I),
		rttr::value("J", zr::Keyboard::J),
		rttr::value("K", zr::Keyboard::K),
		rttr::value("L", zr::Keyboard::L),
		rttr::value("M", zr::Keyboard::M),
		rttr::value("N", zr::Keyboard::N),
		rttr::value("O", zr::Keyboard::O),
		rttr::value("P", zr::Keyboard::P),
		rttr::value("Q", zr::Keyboard::Q),
		rttr::value("R", zr::Keyboard::R),
		rttr::value("S", zr::Keyboard::S),
		rttr::value("T", zr::Keyboard::T),
		rttr::value("U", zr::Keyboard::U),
		rttr::value("V", zr::Keyboard::V),
		rttr::value("W", zr::Keyboard::W),
		rttr::value("X", zr::Keyboard::X),
		rttr::value("Y", zr::Keyboard::Y),
		rttr::value("Z", zr::Keyboard::Z),
		rttr::value("LBracket", zr::Keyboard::LBracket),
		rttr::value("Backslash", zr::Keyboard::Backslash),
		rttr::value("RBracket", zr::Keyboard::RBracket),
		rttr::value("Grave", zr::Keyboard::Grave),
		rttr::value("Escape", zr::Keyboard::Escape),
		rttr::value("Enter", zr::Keyboard::Enter),
		rttr::value("Tab", zr::Keyboard::Tab),
		rttr::value("Backspace", zr::Keyboard::Backspace),
		rttr::value("Insert", zr::Keyboard::Insert),
		rttr::value("Delete", zr::Keyboard::Delete),
		rttr::value("Right", zr::Keyboard::Right),
		rttr::value("Left", zr::Keyboard::Left),
		rttr::value("Down", zr::Keyboard::Down),
		rttr::value("Up", zr::Keyboard::Up),
		rttr::value("PageUp", zr::Keyboard::PageUp),
		rttr::value("PageDown", zr::Keyboard::PageDown),
		rttr::value("Home", zr::Keyboard::Home),
		rttr::value("End", zr::Keyboard::End),
		rttr::value("CapsLock", zr::Keyboard::CapsLock),
		rttr::value("ScrollLock", zr::Keyboard::ScrollLock),
		rttr::value("NumLock", zr::Keyboard::NumLock),
		rttr::value("PrintScreen", zr::Keyboard::PrintScreen),
		rttr::value("Pause", zr::Keyboard::Pause),
		rttr::value("F1", zr::Keyboard::F1),
		rttr::value("F2", zr::Keyboard::F2),
		rttr::value("F3", zr::Keyboard::F3),
		rttr::value("F4", zr::Keyboard::F4),
		rttr::value("F5", zr::Keyboard::F5),
		rttr::value("F6", zr::Keyboard::F6),
		rttr::value("F7", zr::Keyboard::F7),
		rttr::value("F8", zr::Keyboard::F8),
		rttr::value("F9", zr::Keyboard::F9),
		rttr::value("F10", zr::Keyboard::F10),
		rttr::value("F11", zr::Keyboard::F11),
		rttr::value("F12", zr::Keyboard::F12),
		rttr::value("F13", zr::Keyboard::F13),
		rttr::value("F14", zr::Keyboard::F14),
		rttr::value("F15", zr::Keyboard::F15),
		rttr::value("F16", zr::Keyboard::F16),
		rttr::value("F17", zr::Keyboard::F17),
		rttr::value("F18", zr::Keyboard::F18),
		rttr::value("F19", zr::Keyboard::F19),
		rttr::value("F20", zr::Keyboard::F20),
		rttr::value("F21", zr::Keyboard::F21),
		rttr::value("F22", zr::Keyboard::F22),
		rttr::value("F23", zr::Keyboard::F23),
		rttr::value("F24", zr::Keyboard::F24),
		rttr::value("F25", zr::Keyboard::F25),
		rttr::value("Numpad0", zr::Keyboard::Numpad0),
		rttr::value("Numpad1", zr::Keyboard::Numpad1),
		rttr::value("Numpad2", zr::Keyboard::Numpad2),
		rttr::value("Numpad3", zr::Keyboard::Numpad3),
		rttr::value("Numpad4", zr::Keyboard::Numpad4),
		rttr::value("Numpad5", zr::Keyboard::Numpad5),
		rttr::value("Numpad6", zr::Keyboard::Numpad6),
		rttr::value("Numpad7", zr::Keyboard::Numpad7),
		rttr::value("Numpad8", zr::Keyboard::Numpad8),
		rttr::value("Numpad9", zr::Keyboard::Numpad9),
		rttr::value("Decimal", zr::Keyboard::Decimal),
		rttr::value("Divide", zr::Keyboard::Divide),
		rttr::value("Multiply", zr::Keyboard::Multiply),
		rttr::value("Subtract", zr::Keyboard::Subtract),
		rttr::value("Add", zr::Keyboard::Add),
		rttr::value("NumpadEnter", zr::Keyboard::NumpadEnter),
		rttr::value("NumpadEqual", zr::Keyboard::NumpadEqual),
		rttr::value("LShift", zr::Keyboard::LShift),
		rttr::value("LControl", zr::Keyboard::LControl),
		rttr::value("LAlt", zr::Keyboard::LAlt),
		rttr::value("LSuper", zr::Keyboard::LSuper),
		rttr::value("RShift", zr::Keyboard::RShift),
		rttr::value("RControl", zr::Keyboard::RControl),
		rttr::value("RAlt", zr::Keyboard::RAlt),
		rttr::value("RSuper", zr::Keyboard::RSuper),
		rttr::value("Menu", zr::Keyboard::Menu)
	);

	reg::enumeration<zr::MouseButton>("MouseButton")(
		rttr::value("Button0", zr::MouseButton::Button0),
		rttr::value("Button1", zr::MouseButton::Button1),
		rttr::value("Button2", zr::MouseButton::Button2),
		rttr::value("Button3", zr::MouseButton::Button3),
		rttr::value("Button4", zr::MouseButton::Button4),
		rttr::value("Button5", zr::MouseButton::Button5),
		rttr::value("Button6", zr::MouseButton::Button6),
		rttr::value("Button7", zr::MouseButton::Button7),
		rttr::value("ButtonLast", zr::MouseButton::ButtonLast),
		rttr::value("ButtonLeft", zr::MouseButton::ButtonLeft),
		rttr::value("ButtonRight", zr::MouseButton::ButtonRight),
		rttr::value("ButtonMiddle", zr::MouseButton::ButtonMiddle)
	);

	reg::enumeration<zr::EventType>("EventType")(
		rttr::value("None", zr::EventType::None),
		rttr::value("WindowClose", zr::EventType::WindowClose),
		rttr::value("WindowResize", zr::EventType::WindowResize),
		rttr::value("WindowMove", zr::EventType::WindowMove),
		rttr::value("WindowGainedFocus", zr::EventType::WindowGainedFocus),
		rttr::value("WindowLostFocus", zr::EventType::WindowLostFocus),
		rttr::value("KeyPressed", zr::EventType::KeyPressed),
		rttr::value("KeyReleased", zr::EventType::KeyReleased),
		rttr::value("KeyTyped", zr::EventType::KeyTyped),
		rttr::value("MouseButtonPressed", zr::EventType::MouseButtonPressed),
		rttr::value("MouseButtonReleased", zr::EventType::MouseButtonReleased),
		rttr::value("MouseMove", zr::EventType::MouseMove),
		rttr::value("MouseScroll", zr::EventType::MouseScroll),
		rttr::value("ApplicationEvent", zr::EventType::ApplicationEvent)
	);

	rttr::type::register_converter_func(KeyboardToInt);
	rttr::type::register_converter_func(IntToKeyboard);

	/*rttr::type::register_converter_func([](zr::Keyboard keyCode, bool& ok) -> int {
		ok = true;
		return (int)keyCode;
	});*/

	rttr::type::register_converter_func([](zr::MouseButton mouseButtonCode, bool& ok) -> int {
		ok = true;
		return (int)mouseButtonCode;
	});

	rttr::type::register_converter_func([](zr::EventType eventType, bool& ok) -> int {
		ok = true;
		return (int)eventType;
	});
}
