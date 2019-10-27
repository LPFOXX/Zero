#pragma once

#include <string>
#include <vector>

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
		virtual void setModelTransform(const glm::mat4& modelTranform) = 0;
		virtual bool setAnimation(const std::string& animationName) = 0;
		virtual bool setAnimation(unsigned animationIndex) = 0;
		virtual bool getAvailableAnimations(std::vector<std::string>& animations) = 0;
	};
}
