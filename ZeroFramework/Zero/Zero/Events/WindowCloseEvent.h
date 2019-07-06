#pragma once

#include "Event.h"

namespace zr
{
	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent();
		virtual ~WindowCloseEvent();

		virtual unsigned getFamilyFlags() const override;

		virtual EventType getType() const override;
		static EventType getStaticType();
		virtual const char* getName() const override;
	};
}
