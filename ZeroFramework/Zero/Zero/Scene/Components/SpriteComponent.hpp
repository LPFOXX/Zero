#pragma once

#include "zr_pch.h"

namespace zr
{
	struct SpriteComponent
	{
		SpriteComponent() : Color(1.f, 1.f, 1.f, 1.f)
		{
		}

		glm::vec4 Color;
	};
}
