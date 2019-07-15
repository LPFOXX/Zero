#include <zr_pch.h>

#include "KeyPressedEvent.h"

namespace zr
{
	KeyPressedEvent::KeyPressedEvent(int keyCode, int repeatCount) :
		KeyEvent(keyCode),
		mRepeatCount(repeatCount)
	{
	}
	
	KeyPressedEvent::~KeyPressedEvent()
	{
		
	}
	
	EventType KeyPressedEvent::GetStaticType()
	{
		return EventType::KeyPressed;
	}
	
	EventType KeyPressedEvent::getType() const
	{
		return GetStaticType();
	}
	
	const char* KeyPressedEvent::getName() const
	{
		return "KeyPressed";
	}
	
	int KeyPressedEvent::getRepeatCount() const
	{
		return mRepeatCount;
	}
	
	std::string KeyPressedEvent::toString() const
	{
		return "KeyPressed: " + std::to_string(mKeyCode) + " (" + std::to_string(mRepeatCount) + " repeats)";
	}
}
