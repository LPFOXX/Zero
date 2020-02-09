#pragma once

#include "Event.h"

namespace zr
{
	class MouseScrollEvent : public Event
	{
	public:
		MouseScrollEvent(float xOffset, float yOffset);
		virtual ~MouseScrollEvent();

		float getXOffset() const;
		float getYOffset() const;

		virtual EventType getType() const override;
		static EventType GetStaticType();
		virtual const char* getName() const override;
		virtual unsigned getFamilyFlags() const override;
		virtual std::string toString() const override;

	private:
		float mXOffset;
		float mYOffset;
	};
}
