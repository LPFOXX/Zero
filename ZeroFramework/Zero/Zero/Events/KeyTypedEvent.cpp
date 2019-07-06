#include <zr_pch.h>

#include "KeyTypedEvent.h"

namespace zr
{
	KeyTypedEvent::KeyTypedEvent(int keyCode) :
		KeyEvent(keyCode)
	{
	}

	KeyTypedEvent::~KeyTypedEvent()
	{
	}

	EventType KeyTypedEvent::getStaticType()
	{
		return EventType::KeyTyped;
	}

	EventType KeyTypedEvent::getType() const
	{
		return getStaticType();
	}

	const char* KeyTypedEvent::getName() const
	{
		return "KeyTyped";
	}

	unsigned KeyTypedEvent::getFamilyFlags() const
	{
		return EventFamily::InputEvent | EventFamily::KeyboadEvent;
	}

	std::string KeyTypedEvent::toString() const
	{
		return "KeyTyped: " + std::to_string(mKeyCode);
	}
}
