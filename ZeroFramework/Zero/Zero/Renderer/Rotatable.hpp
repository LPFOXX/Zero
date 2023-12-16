#pragma once

#include "../../vendor/glm/include/glm/glm.hpp"
#include "../../vendor/glm/include/glm/gtc/matrix_transform.hpp"

namespace zr
{
	class Rotatable
	{
	public:
		Rotatable() :
			mRotationAngle(0.f),
			mRotationAxis(0.f, 0.f, 1.f),
			mNeedsRotationUpdate(true)
		{

		}

		virtual ~Rotatable() = default;

		void setRotationAngle(float angle)
		{
			mRotationAngle = angle;
			mNeedsRotationUpdate = true;
		}

		void rotate(float offsetAngle)
		{
			mRotationAngle += offsetAngle;
			mNeedsRotationUpdate = true;
		}

		inline float getRotationAngle() const
		{
			return mRotationAngle;
		}

		inline const glm::vec3& getRotationAxis() const
		{
			return mRotationAxis;
		}

		const glm::mat4 getRotationMatrix() const
		{
			mNeedsRotationUpdate = false;
			return glm::rotate(glm::mat4(1.f), mRotationAngle, mRotationAxis);
		}

	protected:
		float mRotationAngle;	/**< Rotation angle in radians. */
		glm::vec3 mRotationAxis;
		mutable bool mNeedsRotationUpdate;
	};

	class Rotatable2D : public Rotatable
	{
	public:
		Rotatable2D() :
			Rotatable()
		{

		}

		virtual ~Rotatable2D() = default;
	};

	class Rotatable3D : public Rotatable
	{
	public:
		Rotatable3D() :
			Rotatable()
		{

		}

		virtual ~Rotatable3D() = default;

		void setRotationAxis(const glm::vec3& axis)
		{
			mRotationAxis = axis;
		}
	};
}
