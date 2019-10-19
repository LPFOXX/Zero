#pragma once

#include "Event.h"

namespace zr
{
	class MouseButtonEvent : public Event
	{
	public:
		virtual ~MouseButtonEvent();

		inline int getButton() const
		{
			return mButton;
		}
		virtual unsigned getFamilyFlags() const override;

	protected:
		MouseButtonEvent(int button);

	protected:
		int mButton;
	};
}
