#include <zr_pch.h>

#include  "WindowGainedFocusEvent.h"

namespace zr
{
	WindowGainedFocusEvent::WindowGainedFocusEvent() :
		Event()
	{
	}

	WindowGainedFocusEvent::~WindowGainedFocusEvent()
	{
	}

	unsigned WindowGainedFocusEvent::getFamilyFlags() const
	{
		return EventFamily::ApplicationEvent;
	}

	EventType WindowGainedFocusEvent::getStaticType()
	{
		return EventType::WindowGainedFocus;
	}

	EventType WindowGainedFocusEvent::getType() const
	{
		return getStaticType();
	}

	const char* WindowGainedFocusEvent::getName() const
	{
		return "WindowGainedFocus";
	}

	std::string WindowGainedFocusEvent::toString() const
	{
		return "WindowGainedFocus";
	}
}
