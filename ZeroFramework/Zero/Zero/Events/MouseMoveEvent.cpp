#include <zr_pch.h>

#include "MouseMoveEvent.h"

namespace zr
{
	MouseMoveEvent::MouseMoveEvent(float x, float y) :
		Event(),
		mX(x),
		mY(y)
	{
		
	}
	
	MouseMoveEvent::~MouseMoveEvent()
	{

	}
		
	EventType MouseMoveEvent::getType() const
	{
		return getStaticType();
	}
	
	EventType MouseMoveEvent::getStaticType()
	{
		return EventType::MouseMove;
	}
	
	const char* MouseMoveEvent::getName() const
	{
		return "MouseMove";
	}
	
	unsigned MouseMoveEvent::getFamilyFlags() const
	{
		return EventFamily::MouseEvent | EventFamily::InputEvent;
	}
	
	std::string MouseMoveEvent::toString() const
	{
		return "MouseMove: " + std::to_string(mX) + ", " + std::to_string(mY);
	}
}
