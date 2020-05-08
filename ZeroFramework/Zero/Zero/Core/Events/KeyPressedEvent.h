#pragma once

#include "KeyEvent.h"

namespace zr
{
	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keyCode, int repeatCount);
		virtual ~KeyPressedEvent();

		virtual EventType getType() const override;
		static EventType GetStaticType();
		virtual const char* getName() const override;
		virtual std::string toString() const override;
		int getRepeatCount() const;

	private:
		int mRepeatCount;
	};
}
