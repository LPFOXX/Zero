#include <zr_pch.h>

#include "WindowResizeEvent.h"

namespace zr
{
	WindowResizeEvent::WindowResizeEvent(unsigned width, unsigned height) :
		mWidth(width),
		mHeight(height)
	{
		
	}
	
	WindowResizeEvent::~WindowResizeEvent()
	{
		
	}
	
	unsigned WindowResizeEvent::getFamilyFlags() const
	{
		return EventFamily::ApplicationEvent;
	}
	
	EventType WindowResizeEvent::getType() const
	{
		return getStaticType();
	}
	
	EventType WindowResizeEvent::getStaticType()
	{
		return EventType::WindowResize;
	}
	
	const char* WindowResizeEvent::getName() const
	{
		return "WindowResize";
	}
	
	std::string WindowResizeEvent::toString() const
	{
		return "WindowResize: " + std::to_string(mWidth) + ", " + std::to_string(mHeight);
	}
}
