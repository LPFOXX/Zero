#pragma once

#include <string>

#include "glm/glm.hpp"

#include "../Renderer/Transformable.hpp"
//#include "../ImGui/ImGuiWidgetComponent.hpp"

namespace zr
{
	struct SpriteComponent
	{
		SpriteComponent() :
			Color(1.f, 1.f, 1.f, 1.f)
		{

		}

		glm::vec4 Color;
	};

	struct TagComponent
	{
		TagComponent() :
			Tag(std::string())
		{
		}

		virtual ~TagComponent() = default;

		std::string Tag;
	};
}
