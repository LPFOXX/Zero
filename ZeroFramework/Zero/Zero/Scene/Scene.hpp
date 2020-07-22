#pragma once

#include "entt.hpp"
#include "glm/glm.hpp"

#include "Zero/Core/Clock.h" 

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

	struct Transformable
	{
		glm::mat4 Transform;
	};

	class Scene
	{
	public:
		Scene() :
			mRegistry()
		{

		}

		virtual ~Scene() = default;

		void onUpdate(const Time& deltaTime)
		{

		}

		void function()
		{
			auto entity = mRegistry.create();
			//mRegistry.emplace<Transformable>(entity, );
		}

	private:
		entt::registry mRegistry;
	};
}
