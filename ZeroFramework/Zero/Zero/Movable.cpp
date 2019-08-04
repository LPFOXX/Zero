#include <zr_pch.h>

#include "Movable.h"

namespace zr
{
	Movable::Movable() :
		mPosition(0.f, 0.f)
	{

	}

	Movable::~Movable()
	{
	}

	void Movable::move(const glm::vec2& offset)
	{
		move(offset.x, offset.y);
	}

	void Movable::move(float xOffset, float yOffset)
	{
		mPosition.x += xOffset;
		mPosition.y += yOffset;

		onPositionUpdate();
	}

	void Movable::setPosition(const glm::vec2& position)
	{
		setPosition(position.x, position.y);
	}

	void Movable::setPosition(float xPosition, float yPosition)
	{
		mPosition.x = xPosition;
		mPosition.y = yPosition;

		onPositionUpdate();
	}

	const glm::vec2& Movable::getPosition() const
	{
		return mPosition;
	}
}
