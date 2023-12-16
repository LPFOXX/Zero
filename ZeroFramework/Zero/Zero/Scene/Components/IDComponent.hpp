#pragma once

#include "../../Core/UUID.hpp"

namespace zr
{
	struct IDComponent
	{
		IDComponent(UUID uuid) : UUID{ uuid }
		{
		}

		UUID UUID;
	};
}
