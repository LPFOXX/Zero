#pragma once

#include "Scalable.hpp"
#include "Movable.hpp"
#include "Rotatable.hpp"

#include "../../glm/include/glm/gtc/matrix_transform.hpp"

namespace zr
{
	class Transformable
	{
	public:
		Transformable() :
			mTransform(1.f)
		{
		}

		virtual ~Transformable() = default;

		const glm::mat4& getTransformMatrix() const
		{
			transform();
			return mTransform;
		}

	protected:
		virtual void transform() const = 0;

	protected:
		mutable glm::mat4 mTransform;
	};

	class Transformable2D : public Transformable, public Movable2D, public Rotatable2D, public Scalable2D
	{
	public:
		Transformable2D() :
			Transformable(),
			Movable2D(),
			Rotatable2D(),
			Scalable2D()
		{

		}

		virtual ~Transformable2D() = default;

	protected:
		void transform() const override
		{
			if (mNeedsTranslationUpdate || mNeedsRotationUpdate || mNeedsScalingUpdate) {
				mTransform =
					getTranslationMatrix() *
					getRotationMatrix() *
					getScaleMatrix();
			}
		}
	};
}
