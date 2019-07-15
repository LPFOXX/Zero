#include "Event.h"

namespace zr
{
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned width, unsigned height);
		virtual ~WindowResizeEvent();

		inline unsigned getWidth() const
		{
			return mWidth;
		}

		inline unsigned getHeight() const
		{
			return mHeight;
		}

		virtual unsigned getFamilyFlags() const override;

		virtual EventType getType() const override;
		static EventType GetStaticType();
		virtual const char* getName() const override;
		virtual std::string toString() const override;

	private:
		unsigned mWidth;
		unsigned mHeight;
	};
}
