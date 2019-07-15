#include <zr_pch.h>

#include "WindowCloseEvent.h"

namespace zr
{
	WindowCloseEvent::WindowCloseEvent() :
		Event()
	{
		
	}
	
	WindowCloseEvent::~WindowCloseEvent()
	{
		
	}
	
	unsigned WindowCloseEvent::getFamilyFlags() const
	{
		return EventFamily::ApplicationEvent;
	}
	
	EventType WindowCloseEvent::getType() const
	{
		return GetStaticType();
	}
	
	EventType WindowCloseEvent::GetStaticType()
	{
		return EventType::WindowClose;
	}
	
	const char* WindowCloseEvent::getName() const
	{
		return "WindowClose";
	}
}
