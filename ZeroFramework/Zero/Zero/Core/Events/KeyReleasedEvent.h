#pragma once

#include "KeyEvent.h"

namespace zr
{
	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keyCode);
		virtual ~KeyReleasedEvent();

		virtual EventType getType() const override;
		static EventType GetStaticType();
		virtual const char* getName() const override;

		virtual std::string toString() const override;
	};
}
