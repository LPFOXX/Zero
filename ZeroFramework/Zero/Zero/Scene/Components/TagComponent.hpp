#pragma once

#include <string>

namespace zr
{
	struct TagComponent
	{
		TagComponent(const std::string& tag = "Unnamed Entity") : Tag{ tag }
		{
		}

		std::string Tag;
	};
}
