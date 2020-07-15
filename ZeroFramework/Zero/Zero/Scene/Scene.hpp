#pragma once

#include "entt.hpp"

namespace zr
{
	/*struct Component
	{
		Component() = default;
		Component(const Component&& move) = default;
		virtual ~Component() = default;

		Component& operator=(const Component&& copy)
		{
			return *this;
		}
	};*/

	class Scene
	{
	public:
		Scene() :
			mRegistry()
		{

		}

		virtual ~Scene() = default;

	private:
		entt::registry mRegistry;
	};
}
