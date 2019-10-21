#pragma once

#include "../vendor/glm/include/glm/glm.hpp"

#include "Core/Clock.h"

namespace zr
{
	class ModelImpl
	{
	public:
		ModelImpl() = default;
		virtual ~ModelImpl() = default;

		virtual void update(const Time& elapsedTime) = 0;
		virtual void render(const glm::mat4& viewProjectionMatrix) = 0;
	};
}
