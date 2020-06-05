#pragma once

#include <string>

#include "Event.h"

namespace zr
{
	class RenderWindowFramebufferResizeEvent : public Event
	{
	public:
		RenderWindowFramebufferResizeEvent(unsigned width, unsigned height) :
			Event(),
			mWidth(width),
			mHeight(height)
		{
		}

		virtual ~RenderWindowFramebufferResizeEvent() = default;

		// Inherited via Event
		virtual EventType getType() const override
		{
			return RenderWindowFramebufferResizeEvent::GetStaticType();
		}

		static EventType GetStaticType()
		{
			return EventType::RenderWindowFramebufferResize;
		}

		virtual const char* getName() const override
		{
			return "RenderWindowFramebufferResize";
		}

		virtual unsigned getFamilyFlags() const override
		{
			return EventFamily::ApplicationEvent;
		}

		std::string toString() const override
		{
			return "RenderWindowFramebufferResize: " + std::to_string(mWidth) + ", " + std::to_string(mHeight);
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
