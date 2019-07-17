#include "Event.h"

#include "../../vendor/glm/include/glm/glm.hpp"

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

	public:
		static EventType GetStaticType();
		static glm::vec2 GetMovementOffset();

		virtual EventType getType() const override;
		virtual const char* getName() const override;
		virtual unsigned getFamilyFlags() const override;
		virtual std::string toString() const override;

	private:
		static float sLastX;
		static float sLastY;
		static float sOffsetX;
		static float sOffsetY;
		static bool sIsFirstEvent;


	private:
		float mX;
		float mY;
	};
}
