#include <zr_pch.h>

#include "MouseButtonPressedEvent.h"

namespace zr
{
	MouseButtonPressedEvent::MouseButtonPressedEvent(int button) :
		MouseButtonEvent(button)
	{
		
	}
	
	MouseButtonPressedEvent::~MouseButtonPressedEvent()
	{
		
	}
			
	EventType MouseButtonPressedEvent::getType() const
	{
		return getStaticType();
	}
	
	EventType MouseButtonPressedEvent::getStaticType()
	{
		return EventType::MouseButtonPressed;
	}
	
	const char* MouseButtonPressedEvent::getName() const
	{
		return "MouseButtonPressed";
	}
	
	std::string MouseButtonPressedEvent::toString() const
	{
		return "MouseButtonPressed: " +  std::to_string(mButton);
	}
}
