#include <zr_pch.h>

#include "MouseScrollEvent.h"

namespace zr
{
	MouseScrollEvent::MouseScrollEvent(float xOffset, float yOffset) :
		Event(),
		mXOffset(xOffset),
		mYOffset(yOffset)
	{
		
	}
	
	MouseScrollEvent::~MouseScrollEvent()
	{
		
	}
	
	float MouseScrollEvent::getXOffset() const
	{
		return mXOffset;
	}
	
	float MouseScrollEvent::getYOffset() const
	{
		return mYOffset;
	}
	
	EventType MouseScrollEvent::getType() const
	{
		return EventType::MouseScroll;
	}
	
	EventType MouseScrollEvent::GetStaticType()
	{
		return EventType::MouseScroll;
	}
	
	const char* MouseScrollEvent::getName() const
	{
		return "MouseScroll";
	}
	
	unsigned MouseScrollEvent::getFamilyFlags() const
	{
		return EventFamily::MouseEvent | EventFamily::InputEvent;
	}
	
	std::string MouseScrollEvent::toString() const
	{
		return "MouseScroll: " + std::to_string(getXOffset()) + ", " + std::to_string(getYOffset());
	}
}
