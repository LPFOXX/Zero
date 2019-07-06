#include <zr_pch.h>

#include "KeyReleasedEvent.h"

namespace zr
{
	KeyReleasedEvent::KeyReleasedEvent(int keyCode) :
		KeyEvent(keyCode)
	{
		
	}
	
	KeyReleasedEvent::~KeyReleasedEvent()
	{
		
	}	
			
	EventType KeyReleasedEvent::getType() const
	{
		return getStaticType();
	}
	
	EventType KeyReleasedEvent::getStaticType()
	{
		return EventType::KeyReleased;
	}
	
	const char* KeyReleasedEvent::getName() const
	{
		return "KeyReleased";
	}
	
	std::string KeyReleasedEvent::toString() const
	{
		return "KeyReleased: " + std::to_string(mKeyCode);
	}
}
