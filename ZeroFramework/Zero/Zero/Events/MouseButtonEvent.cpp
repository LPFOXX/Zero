#include <zr_pch.h>

#include "MouseButtonEvent.h"

namespace zr
{
	MouseButtonEvent::MouseButtonEvent(int button) :
		Event(),
		mButton(button)
	{
		
	}
	
	MouseButtonEvent::~MouseButtonEvent()
	{
		
	}
	
	
	unsigned MouseButtonEvent::getFamilyFlags() const
	{
		return EventFamily::MouseEvent | EventFamily::InputEvent;
	}
}
