#pragma once

#include "../../vendor/glm/include/glm/glm.hpp"

namespace zr
{
	template <typename T>
	class Movable
	{
	public:
		Movable() :
			mPosition(),
			mNeedsTranslationUpdate(true)
		{

		}

		virtual ~Movable() = default;

		/**
		* @brief Sets the position of the object.
		*
		* @param position Where to put the object.
		*
		*/
		void setPosition(const T& position)
		{
			mNeedsTranslationUpdate = true;
			mPosition = position;
		}

		/**
		* @brief Gets the center position of the object.
		*
		* @returns The posititon of the sprite rect.
		*
		*/
		const T& getPosition() const
		{
			return mPosition;
		}

		/**
		* @brief Moves the object around the axis.
		*
		* A positive offset on the horizontal axis dislocates the sprite to the right. On the other
		* hand it will dislocate to the left when applied a negative offset on the horizontal axis.
		* A positive offset on the vertical axis dislocates the sprite down. On the other
		* hand it will move up when applied a negative offset on the vertical axis.
		*
		* @param offset The offset to apply to the current position of the sprite.
		*
		*/
		inline void move(const T& offset)
		{
			mNeedsTranslationUpdate = true;
			mPosition += offset;
		}

		virtual const glm::mat4 getTranslationMatrix() const = 0;

	protected:
		T mPosition;
		mutable bool mNeedsTranslationUpdate = true;
	};

	class Movable2D : public Movable<glm::vec2>
	{
	public:
		Movable2D() :
			Movable(),
			mDepthLevel(0.f)
		{
			mPosition = { 0.f, 0.f };
		}

		virtual ~Movable2D() = default;

		void setPosition(float x, float y)
		{
			mNeedsTranslationUpdate = true;
			mPosition.x = x;
			mPosition.y = y;
		}

		void move(float offsetX, float offsetY)
		{
			mNeedsTranslationUpdate = true;
			mPosition.x += offsetX;
			mPosition.y += offsetY;
		}

		float getDepthLevel() const
		{
			return mDepthLevel;
		}

		void setDepthLevel(float depthLevel)
		{
			mNeedsTranslationUpdate = true;
			mDepthLevel = depthLevel;
		}

		virtual const glm::mat4 getTranslationMatrix() const override
		{
			mNeedsTranslationUpdate = false;
			return glm::translate(glm::mat4(1.f), glm::vec3(mPosition, mDepthLevel));
		}

	private:
		float mDepthLevel;
	};

	class Movable3D : public Movable<glm::vec3>
	{
	public:
		Movable3D() :
			Movable()
		{
			mPosition = { 0.f, 0.f, 0.f };
		}

		virtual ~Movable3D() = default;

		void setPosition(float x, float y, float z)
		{
			mNeedsTranslationUpdate = true;
			mPosition.x = x;
			mPosition.y = y;
			mPosition.z = z;
		}

		void move(float offsetX, float offsetY, float offsetZ)
		{
			mNeedsTranslationUpdate = true;
			mPosition.x += offsetX;
			mPosition.y += offsetY;
			mPosition.z += offsetZ;
		}

		virtual const glm::mat4 getTranslationMatrix() const override
		{
			mNeedsTranslationUpdate = false;
			return glm::translate(glm::mat4(1.f), mPosition);
		}
	};
}
