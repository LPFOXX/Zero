#pragma once

#include "ModelImpl.h"

namespace zr
{
	class NullModelImpl : public ModelImpl
	{
	public:
		NullModelImpl() = default;
		virtual ~NullModelImpl() = default;

		virtual void render(const glm::mat4& viewProjectionMatrix) override
		{
		}

		virtual void update(const Time& elapsedTime) override
		{
		}

		virtual void setModelTransform(const glm::mat4& modelTranform) override
		{
		}
	};
}