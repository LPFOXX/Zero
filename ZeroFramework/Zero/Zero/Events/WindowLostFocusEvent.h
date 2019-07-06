#pragma once

#include "Event.h"

namespace zr
{
	class WindowLostFocusEvent : public Event
	{
	public:
		WindowLostFocusEvent();
		virtual ~WindowLostFocusEvent();

		virtual unsigned getFamilyFlags() const override;
		static EventType getStaticType();
		virtual EventType getType() const override;
		virtual const char* getName() const override;
		virtual std::string toString() const override;
	};
}
