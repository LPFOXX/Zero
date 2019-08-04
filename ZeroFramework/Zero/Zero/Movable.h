#pragma once

#include "../vendor/glm/include/glm/glm.hpp"

namespace zr
{
	class Movable
	{
	public:
		Movable();
		virtual ~Movable();

		/**
		* @brief Moves the sprite around the X and/or Y axis.
		*
		* A positive offset on the horizontal axis dislocates the sprite to the right. On the other
		* hand it will dislocate to the left when applied a negative offset on the horizontal axis.
		* A positive offset on the vertical axis dislocates the sprite down. On the other
		* hand it will move up when applied a negative offset on the vertical axis.
		*
		* @param offset The offset to apply to the current position of the sprite.
		*
		*/
		virtual void move(const glm::vec2& offset);

		/**
		* @brief Moves the sprite around the X and/or Y axis.
		*
		* A positive offset on the horizontal axis dislocates the sprite to the right. On the other
		* hand it will dislocate to the left when applied a negative offset on the horizontal axis.
		* A positive offset on the vertical axis dislocates the sprite down. On the other
		* hand it will move up when applied a negative offset on the vertical axis.
		*
		* @param xOffset The horizontal offset to dislocate the sprite.
		* @param xOffset The vertical offset to dislocate the sprite.
		*
		*/
		virtual void move(float xOffset, float yOffset);

		/**
		* @brief Sets the position of the sprite.
		*
		* @param position The position to put the sprite in.
		*
		*/
		virtual void setPosition(const glm::vec2& position);

		/**
		* @brief Sets the position of the sprite.
		*
		* @param xPosition The horizontal position to put the sprite in.
		* @param yPosition The vertical position to put the sprite in.
		*
		*/
		virtual void setPosition(float xPosition, float yPosition);

		/**
		* @brief Gets the position of the rect containing the sprite.
		*
		* @returns The posititon of the sprite rect.
		*
		*/
		virtual const glm::vec2& getPosition() const;

	protected:
		virtual void onPositionUpdate() = 0;

	protected:
		glm::vec2 mPosition;
	};
};
