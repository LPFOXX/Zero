#include "zr_pch.h"

#include "Entity.hpp"

namespace zr
{
	Entity::Entity(Scene* scene, entt::entity nativeId) :
		mScene{ scene },
		mNativeId{ nativeId }
	{
	}
}
