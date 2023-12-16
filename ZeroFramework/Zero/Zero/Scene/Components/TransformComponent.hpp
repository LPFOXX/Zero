#pragma once

#include "zr_pch.h"

namespace zr
{
	struct TransformComponent
	{
		TransformComponent() :
			Position{ 0.f, 0.f, 0.f },
			Rotation{},
			Scale{ 1.f, 1.f, 1.f }
		{
		}

		TransformComponent(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) :
			Position(position),
			Rotation(rotation),
			Scale(scale)
		{
		}

		glm::mat4 getTransform() const
		{
			glm::mat4 transform = glm::mat4(1.f);
			return glm::translate(transform, Position) *
				glm::rotate(transform, glm::radians(Rotation.x), {1.f, .0f, 0.f}) *
				glm::rotate(transform, glm::radians(Rotation.y), {0.f, 1.f, .0f}) *
				glm::rotate(transform, glm::radians(Rotation.z), {.0f, 0.f, 1.f}) *
				glm::scale(transform, Scale);
		}

		glm::vec3 Position;
		glm::vec3 Rotation;
		glm::vec3 Scale;
	};
}
