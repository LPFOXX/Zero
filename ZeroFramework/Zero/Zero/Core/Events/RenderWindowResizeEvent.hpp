#pragma once

#include "Event.h"

namespace zr
{
	class RenderWindowResizeEvent : public Event
	{
	public:
		RenderWindowResizeEvent(unsigned width, unsigned height) :
			Event(),
			mWidth(width),
			mHeight(height)
		{
		}

		virtual ~RenderWindowResizeEvent() = default;

		// Inherited via Event
		virtual EventType getType() const override
		{
			return RenderWindowResizeEvent::GetStaticType();
		}

		static EventType GetStaticType()
		{
			return EventType::RenderWindowResize;
		}

		virtual const char* getName() const override
		{
			return "RenderWindowResize";
		}

		virtual unsigned getFamilyFlags() const override
		{
			return EventFamily::ApplicationEvent;
		}

		std::string toString() const override
		{
			return "RenderWindowResize: " + std::to_string(mWidth) + ", " + std::to_string(mHeight);
		}

		unsigned getWidth() const
		{
			return mWidth;
		}

		unsigned getHeight() const
		{
			return mHeight;
		}

	private:
		unsigned mWidth;
		unsigned mHeight;
	};
}
