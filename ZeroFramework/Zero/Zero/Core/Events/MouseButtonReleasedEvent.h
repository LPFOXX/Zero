#pragma once

#include "MouseButtonEvent.h"

namespace zr
{
	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button);
		virtual ~MouseButtonReleasedEvent();

		virtual EventType getType() const override;
		static EventType GetStaticType();
		virtual const char* getName() const override;
		virtual std::string toString() const override;
	};
}
