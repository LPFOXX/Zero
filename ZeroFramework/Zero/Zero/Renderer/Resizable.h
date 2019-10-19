#pragma once

namespace zr
{
	class Resizable
	{
	public:
		Resizable::Resizable() :
			mSize(0.f, 0.f)
		{
		}

		Resizable::~Resizable()
		{
		}

		/**
		* @brief Sets the size of the sprite.
		*
		* @param size The size to change the sprite rect into.
		*
		*/
		void Resizable::setSize(const glm::vec2& size)
		{
			setSize(size.x, size.y);
		}

		/**
		* @brief Sets the size of the sprite.
		*
		* @param width The horizontal size to change the sprite rect into.
		* @param height The vertical size to change the sprite rect into.
		*
		*/
		void Resizable::setSize(float width, float height)
		{
			mSize.x = width;
			mSize.y = height;

			onSizeUpdate();
		}

		/**
		* @brief Gets the size of the sprite.
		*
		* @return The size of the rect containing the sprite.
		*
		*/
		inline const glm::vec2& Resizable::getSize() const
		{
			return mSize;
		}

	protected:
		virtual void onSizeUpdate() = 0;

	protected:
		glm::vec2 mSize;
	};
}
