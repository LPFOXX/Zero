#pragma once

#include "../../vendor/glm/include/glm/glm.hpp"
#include <utility>

namespace zr
{
	template <typename T>
	class Scalable
	{
	public:
		Scalable() :
			mSize(),
			mNeedsScalingUpdate(true)
		{

		}

		virtual ~Scalable() = default;

		virtual bool setSize(const T& newSize) = 0;

		const T& getSize() const
		{
			return mSize;
		}

	protected:
		T mSize;
		mutable bool mNeedsScalingUpdate;
	};

	class Scalable2D : public Scalable<glm::vec2>
	{
	public:
		Scalable2D() :
			Scalable()
		{
			mSize = { 0.f, 0.f };
		}

		virtual ~Scalable2D() = default;

		virtual bool setSize(const glm::vec2& newSize) override
		{
			if (newSize.x < 0.f || newSize.y < 0.f) return false;

			mNeedsScalingUpdate = true;
			mSize = newSize;
			return true;
		}

		bool setSize(float width, float height)
		{
			if (width < 0.f || height < 0.f) return false;

			mNeedsScalingUpdate = true;
			mSize.x = width;
			mSize.y = height;
			return true;
		}

		bool setWidth(float width)
		{
			if (width < 0.f) return false;

			mNeedsScalingUpdate = true;
			mSize.x = width;
			return true;
		}

		bool setHeight(float height)
		{
			if (height < 0.f) return false;

			mNeedsScalingUpdate = true;
			mSize.y = height;
			return true;
		}

		inline const float getWidth() const
		{
			return mSize.x;
		}

		inline const float getHeight() const
		{
			return mSize.y;
		}

		const glm::mat4 getScaleMatrix() const
		{
			mNeedsScalingUpdate = false;
			return glm::scale(glm::mat4(1.f), glm::vec3(mSize, 1.f));
		}
	};

	class Scalable3D : public Scalable<glm::vec3>
	{
		Scalable3D() :
			Scalable()
		{
			mSize = { 0.f, 0.f, 0.f };
		}

		virtual ~Scalable3D() = default;

		virtual bool setSize(const glm::vec3 & newSize) override
		{
			if (newSize.x < 0.f || newSize.y < 0.f || newSize.z < 0.f) return false;

			mSize = newSize;
			return true;
		}

		bool setSize(float width, float height, float zSize)
		{
			if (width < 0.f || height < 0.f || zSize < 0.f) return false;

			mSize.x = width;
			mSize.y = height;
			mSize.z = zSize;
			return true;
		}

		bool setWidth(float width)
		{
			if (width < 0.f) return false;

			mSize.x = width;
			return true;
		}

		bool setHeight(float height)
		{
			if (height < 0.f) return false;

			mSize.y = height;
			return true;
		}
		
		bool setZSize(float zSize)
		{
			if (zSize < 0.f) return false;

			mSize.z = zSize;
			return true;
		}

		inline const float getWidth() const
		{
			return mSize.x;
		}

		inline const float getHeight() const
		{
			return mSize.y;
		}

		inline const float getZSize() const
		{
			return mSize.z;
		}
	};
}
