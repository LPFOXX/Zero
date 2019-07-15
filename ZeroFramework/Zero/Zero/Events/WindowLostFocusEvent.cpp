#include <zr_pch.h>

#include  "WindowLostFocusEvent.h"

namespace zr
{
	WindowLostFocusEvent::WindowLostFocusEvent() :
		Event()
	{
	}

	WindowLostFocusEvent::~WindowLostFocusEvent()
	{
	}

	unsigned WindowLostFocusEvent::getFamilyFlags() const
	{
		return EventFamily::ApplicationEvent;
	}

	EventType WindowLostFocusEvent::GetStaticType()
	{
		return EventType::WindowLostFocus;
	}

	EventType WindowLostFocusEvent::getType() const
	{
		return GetStaticType();
	}

	const char* WindowLostFocusEvent::getName() const
	{
		return "WindowLostFocus";
	}

	std::string WindowLostFocusEvent::toString() const
	{
		return "WindowLostFocus";
	}
}
