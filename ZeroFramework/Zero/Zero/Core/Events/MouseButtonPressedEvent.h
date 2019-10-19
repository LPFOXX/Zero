#pragma once

#include "MouseButtonEvent.h"

namespace zr
{
	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button);
		virtual ~MouseButtonPressedEvent();

		virtual EventType getType() const override;
		static EventType GetStaticType();
		virtual const char* getName() const override;
		virtual std::string toString() const override;
	};
}
