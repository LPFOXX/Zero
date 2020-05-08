#pragma once

#include <iostream>

#include <rttr/registration>
#include <rttr/registration_friend.h>

namespace test
{
	class Sprite
	{
		RTTR_ENABLE();
		RTTR_REGISTRATION_FRIEND

	public:
		Sprite() :
			mX(0.f),
			mY(0.f),
			mWidth(0.f),
			mHeight(0.f)
		{
		}

		~Sprite()
		{
		}

		void setPosition(float x, float y)
		{
			mX = x;
			mY = y;
		}

		void getPosition(float& x, float& y) const
		{
			x = mX;
			y = mY;
		}

		std::pair<float, float> getPosition() const
		{
			return { mX, mY };
		}

		void setSize(float width, float height)
		{
			if (width < 0.f || height < 0.f) return;

			mWidth = width;
			mHeight = height;
		}

		void getSize(float& width, float& height) const
		{
			width = mWidth;
			height = mHeight;
		}

		std::pair<float, float> getSize() const
		{
			return { mWidth, mHeight };
		}

		void move(float xOffset, float yOffset)
		{
			mX += xOffset;
			mY += yOffset;
		}

		void draw() const
		{
			std::cout << "X: " << mX << ", Y: " << mY << "\n";
			std::cout << "Width: " << mWidth << ", Height: " << mHeight << "\n";
		}

	private:
		float mX, mY;
		float mWidth, mHeight;

	public:
		float PublicX = 0.f, PublicY = 0.f;
	};
}