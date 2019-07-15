#include <zr_pch.h>

#include "MouseMoveEvent.h"

namespace zr
{
	float MouseMoveEvent::sLastX = 0.f;
	float MouseMoveEvent::sLastY = 0.f;
	float MouseMoveEvent::sOffsetX = 0.f;
	float MouseMoveEvent::sOffsetY = 0.f;

	MouseMoveEvent::MouseMoveEvent(float x, float y) :
		Event(),
		mX(x),
		mY(y)
	{
		MouseMoveEvent::sOffsetX = mX - MouseMoveEvent::sLastX;
		MouseMoveEvent::sOffsetY = mY - MouseMoveEvent::sLastY;
		MouseMoveEvent::sLastX = mX;
		MouseMoveEvent::sLastY = mY;
	}

	MouseMoveEvent::~MouseMoveEvent()
	{

	}

	EventType MouseMoveEvent::getType() const
	{
		return GetStaticType();
	}

	EventType MouseMoveEvent::GetStaticType()
	{
		return EventType::MouseMove;
	}

	glm::vec2 MouseMoveEvent::GetMovementOffset()
	{
		return { MouseMoveEvent::sOffsetX , MouseMoveEvent::sOffsetY };
	}

	const char* MouseMoveEvent::getName() const
	{
		return "MouseMove";
	}

	unsigned MouseMoveEvent::getFamilyFlags() const
	{
		return EventFamily::MouseEvent | EventFamily::InputEvent;
	}

	std::string MouseMoveEvent::toString() const
	{
		return "MouseMove: " + std::to_string(mX) + ", " + std::to_string(mY);
	}
}
