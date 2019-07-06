#include <zr_pch.h>

#include "MouseButtonReleasedEvent.h"

namespace zr
{
	MouseButtonReleasedEvent::MouseButtonReleasedEvent(int button) :
		MouseButtonEvent(button)
	{
		
	}
	
	MouseButtonReleasedEvent::~MouseButtonReleasedEvent()
	{
		
	}
			
	EventType MouseButtonReleasedEvent::getType() const
	{
		return getStaticType();
	}
	
	EventType MouseButtonReleasedEvent::getStaticType()
	{
		return EventType::MouseButtonReleased;
	}
	
	const char* MouseButtonReleasedEvent::getName() const
	{
		return "MouseButtonRelease";
	}
	
	std::string MouseButtonReleasedEvent::toString() const
	{
		return "MouseButtonRelease: " +  std::to_string(mButton);
	}
}
