#include "Event.h"

namespace zr
{
	class MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(float x, float y);
		virtual ~MouseMoveEvent();

		inline float getX() const
		{
			return mX;
		}
		inline float getY() const
		{
			return mY;
		}

		virtual EventType getType() const override;
		static EventType getStaticType();
		virtual const char* getName() const override;
		virtual unsigned getFamilyFlags() const override;
		virtual std::string toString() const override;

	private:
		float mX;
		float mY;
	};
}
