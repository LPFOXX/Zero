#pragma once

#include "Event.h"

namespace zr
{
	class KeyEvent : public Event
	{
	public:
		virtual ~KeyEvent();
		inline int getKeyCode() const
		{
			return mKeyCode;
		}

		virtual unsigned getFamilyFlags() const override;

	protected:
		KeyEvent(int keyCode);

	protected:
		int mKeyCode;
	};
}
