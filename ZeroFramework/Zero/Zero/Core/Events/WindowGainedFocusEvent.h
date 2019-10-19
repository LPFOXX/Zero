#pragma once

#include "Event.h"

namespace zr
{
	class WindowGainedFocusEvent : public Event
	{
	public:
		WindowGainedFocusEvent();
		virtual ~WindowGainedFocusEvent();

		virtual unsigned getFamilyFlags() const override;
		static EventType getStaticType();
		virtual EventType getType() const override;
		virtual const char* getName() const override;
		virtual std::string toString() const override;
	};
}
