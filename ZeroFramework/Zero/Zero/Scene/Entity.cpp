#include "zr_pch.h"

#include "Entity.hpp"

namespace zr
{
	Entity::Entity() :
		mNativeId(entt::null),
		mRegistry(nullptr)
	{

	}

	Entity::Entity(entt::registry& registry, const entt::entity& entity) :
		mNativeId(entity),
		mRegistry(&registry)
	{

	}
}
