#include <zr_pch.h>

#include "ApplicationEvent.h"

namespace zr
{
	ApplicationEvent::ApplicationEvent() :
		Event()
	{
		
	}
	
	ApplicationEvent::~ApplicationEvent()
	{
		
	}
	
	EventType ApplicationEvent::getStaticType()
	{
		return EventType::ApplicationEvent;
	}

	EventType ApplicationEvent::getType() const
	{
		return getStaticType();
	}
	
	const char* ApplicationEvent::getName() const
	{
		return "ApplicationEvent";
	}
	
	unsigned ApplicationEvent::getFamilyFlags() const
	{
		return EventFamily::ApplicationEvent;
	}
}
