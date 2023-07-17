#pragma once

#include "../Core/Core.h"
#include "entt.hpp"

namespace zr
{
	class Entity
	{
	public:
		virtual ~Entity() = default;

		template <typename ComponentType, typename... Args>
		ComponentType& attachComponent(Args&&... args)
		{
			return mRegistry->emplace_or_replace<ComponentType>(mNativeId, std::forward<Args>(args)...);
		}

		template <typename ComponentType>
		void detachComponent()
		{
			mRegistry->remove_if_exists<ComponentType>(mNativeId);
		}

		template <typename ComponentType>
		ComponentType& getComponent()
		{
			return mRegistry->get<ComponentType>(mNativeId);
		}

		template <typename ComponentType>
		bool hasComponent()
		{
			return mRegistry->has<ComponentType>(mNativeId);
		}

		operator bool()
		{
			return mRegistry->valid(mNativeId);
		}

	private:
		friend class Scene;
		Entity();
		Entity(entt::registry& registry, const entt::entity& nativeId);

	private:
		entt::entity mNativeId;
		entt::registry* mRegistry;
	};
}
